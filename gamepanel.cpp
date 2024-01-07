#include "gamepanel.h"
#include <QPropertyAnimation>
#include <QMouseEvent>
#include "cardpanel.h"
#include "playhand.h"
#include "ui_gamepanel.h"
#include <QRandomGenerator>
#include <QPainter>
#include <QDebug>
#include <ostream>
#include "endingpanel.h"
#include "userplayer.h"
#include "gamecontrol.h"

GamePanel::GamePanel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GamePanel)
{
    ui->setupUi(this);
    // 1. 背景图
    int num = QRandomGenerator::global()->bounded(10);
    QString path = QString(":/images/background-%1.png").arg(num+1);
    m_bkImage.load(path);

    // 2. 窗口的标题的大小
    this->setWindowTitle("欢乐斗地主");
    this->setFixedSize(1000, 650);

    // 3. 实例化游戏控制类对象
    gameControlInit();

    // 4. 玩家得分(更新)
    updatePlayerScore();
    // 5. 切割游戏图片
    initCardMap();

    // 6. 初始化游戏中的按钮组
    initButtonsGroup();

    // 7. 初始化玩家在窗口中的上下文环境
    initPlayerContext();

    // 8. 扑克牌场景初始化
    initGameScene();


    // 9. 倒计时窗口初始化
    initCountDown();
      //定时初始化
    m_timer = new QTimer(this);
    //初始化动画类
    m_animation = new AnimationWindow(this);
    //定时器链接发牌处理函数
    connect(m_timer,&QTimer::timeout,this, &GamePanel::onDispatchCard);
    //bgm声音类初始化
    m_bgm = new BGMControl(this);
}

GamePanel::~GamePanel()
{
    delete ui;
}

void GamePanel::gameControlInit()
{
    gc = new GameControl(this);
    gc->playerInint();
    //得到3个玩家的实例化对象
    PlayerList<<gc->getLeftRobt();
    PlayerList<<gc->getRightRobt();
    PlayerList<<gc->getUserPlayer();
    //存储的顺序左侧机器人右侧机器人当前玩
    //当游戏玩家状态改变后发出信号调用游戏面板的改变函数属于叫地主的处理函数
    connect(gc,&GameControl::playerStatusChanged,this,&GamePanel::onPlayerStatusChanged);
    //下注时的展示抢地主不抢等动画
    connect(gc,&GameControl::notifyGrabLordBet,this,&GamePanel::onGrabLordBet);
    //状态转换的处理函数
    connect(gc, &GameControl::gameStatusChanged, this, &GamePanel::gameStatusPrecess);
    //显示出的牌到界面
    connect(gc, &GameControl::notifyPlayHand, this, &GamePanel::onDisposePlayHand);

    connect(gc->getLeftRobt(), &Player::notifyPickCards, this, &GamePanel::disposeCard);
    connect(gc->getRightRobt(), &Player::notifyPickCards, this, &GamePanel::disposeCard);
    connect(gc->getUserPlayer(), &Player::notifyPickCards, this, &GamePanel::disposeCard);
}

void GamePanel::updatePlayerScore()
{
    int left = PlayerList[0]->getScore();
    int right = PlayerList[1]->getScore();
    int user = PlayerList[2]->getScore();
    ui->scorePanelsEff->setScores(left,right,user);
}
void GamePanel::onCardSelected(Qt::MouseButton button)
{
    // 1. 判断是不是出牌状态
    if(stat == GameControl::callLords ||
        stat == GameControl::sendCard)
    {
        return;
    }
    // 2. 判断发出信号的牌的所有者是不是当前用户玩家
    CardPanel* panel = static_cast<CardPanel*>(sender());
    if(panel->getPlayer() != gc->getUserPlayer())
    {
        return;
    }
    // 3. 保存当前被选中的牌的窗口对象
    m_curSelCard = panel;
    // 4. 判断参数的鼠标键是左键还是右键
    if(button == Qt::LeftButton)
    {
        // 设置扑克牌的选中状态
        panel->setSeleted(!panel->isSelected());
        // 更新扑克牌在窗口中的显示
        updatePlayerCards(panel->getPlayer());
        // 保存或删除扑克牌窗口对象
        QSet<CardPanel*>::const_iterator it = m_selectCards.find(panel);
        if(it == m_selectCards.constEnd())
        {
            m_selectCards.insert(panel);
        }
        else
        {
            m_selectCards.erase(it);
        }
        m_bgm->playAssistMusic(BGMControl::SelectCard);
    }
    else if(button == Qt::RightButton)
    {
        // 调用出牌按钮的槽函数
        onUserPlayHand();
    }
}
void GamePanel::initCardMap()
{
    // 1. 加载大图
    QPixmap pixmap(":/images/card.png");
    // 2. 计算每张图片大小
    gc->m_cardSize.setWidth(pixmap.width() / 13);
    gc->m_cardSize.setHeight(pixmap.height() / 5);

    // 3. 背景图
    gc->m_cardBackImg = pixmap.copy(2*gc->m_cardSize.width(), 4*gc->m_cardSize.height(),
                                    gc->m_cardSize.width(), gc->m_cardSize.height());
    // 正常花色
    for(int i=0, suit=Card::SuitBegin+1; suit<Card::SuitEnd; ++suit, ++i)
    {
        for(int j=0, pt=Card::PointBegin+1; pt<Card::Card_SJ; ++pt, ++j)
        {
            Card card((Card::CardPoint)pt, (Card::CardSuit)suit);
            CardPanel * panel = new CardPanel(this);
            // 裁剪图片
            gc->cropImage(pixmap, panel,j*gc->m_cardSize.width(), i*gc->m_cardSize.height(), card);
            this->CardPenalMap.insert(card,panel);
            connect(panel,&CardPanel::cardSelected,this,&GamePanel::onCardSelected);
        }
    }
    // 大小王
    Card c;
    c.setPoint(Card::Card_SJ);
    c.setSuit(Card::SuitBegin);
    CardPanel *panel1 = new CardPanel(this);
    CardPanel *panel2 = new CardPanel(this);
    gc->cropImage(pixmap, panel1,0, 4*gc->m_cardSize.height(), c);
    this->CardPenalMap.insert(c,panel1);
    connect(panel1,&CardPanel::cardSelected,this,&GamePanel::onCardSelected);

    c.setPoint(Card::Card_BJ);
    gc->cropImage(pixmap, panel2,gc->m_cardSize.width(), 4*gc->m_cardSize.height(), c);
    this->CardPenalMap.insert(c,panel2);
    connect(panel2,&CardPanel::cardSelected,this,&GamePanel::onCardSelected);
}

void GamePanel::initPlayerContext()
{
    // 1. 放置玩家扑克牌的区域
    const QRect cardsRect[] =
        {
        // x, y, width, height
        QRect(90, 130, 100, height() - 200),                    // 左侧机器人
        QRect(rect().right() - 190, 130, 100, height() - 200),  // 右侧机器人
        QRect(250, rect().bottom() - 120, width() - 500, 100)   // 当前玩家
    };
    // 2. 玩家出牌的区域
    const QRect playHandRect[] =
        {
        QRect(260, 150, 100, 100),                              // 左侧机器人
        QRect(rect().right() - 360, 150, 100, 100),             // 右侧机器人
        QRect(150, rect().bottom() - 290, width() - 300, 105)   // 当前玩家
    };
    // 3. 玩家头像显示的位置
    const QPoint roleImgPos[] =
        {
        QPoint(cardsRect[0].left()-80, cardsRect[0].height() / 2 + 20),     // 左侧机器人
        QPoint(cardsRect[1].right()+10, cardsRect[1].height() / 2 + 20),    // 右侧机器人
        QPoint(cardsRect[2].right()-10, cardsRect[2].top() - 10)            // 当前玩家
    };
    //获取用户在list中的下标
    int index = PlayerList.indexOf(gc->getUserPlayer());
    for(int i =0;i<PlayerList.size();i++)
    {
        PlayerContext context;
        //保存玩家头像区域
        QLabel *role = new QLabel(this);
        role->resize(84,120);
        context.roleImg = role;
        role->hide();
        role->move(roleImgPos[i]);
        //保存牌显示水平信息，牌正面还是背面
        context.align = (index == i) ? CardAlign::Horizontal:CardAlign::Vertical;
        context.isFrontSide = (index == i)? true:false;
        //保存放置扑克牌的区域
        context.cardRect = cardsRect[i];
        //保存出牌位置
        context.playHandRect = playHandRect[i];
        //保存提示信息
        QLabel * infor = new QLabel(this);
        infor->resize(160,98);
        infor->hide();
        context.info = infor;
        QRect rect = playHandRect[i];
        QPoint pt(rect.left() + (rect.width() - context.info->width()) / 2,
                  rect.top() + (rect.height() - context.info->height()) / 2);
        //把提示信息放到各自的出牌区域的中位置
        context.info->move(pt);
        m_contextMap.insert(PlayerList[i],context);
    }
}

void GamePanel::initButtonsGroup()
{
    //初始化按钮组
    ui->butGroup->initButtons();
    ui->butGroup->selectPanel(ButtonGroup::Start);
    //单击start按钮后的启动函数
    connect(ui->butGroup, &ButtonGroup::startGame, this, [=](){
        // 界面的初始化
        ui->butGroup->selectPanel(ButtonGroup::Empty);
        gc->clearPlayerScore();
        updatePlayerScore();
        // 修改游戏状态 -> 发牌
        gameStatusPrecess(GameControl::sendCard);
        // 播放背景音乐
        m_bgm->startBGM(80);
    });

    //处理发牌 不要 和下注对应的信号函数
    connect(ui->butGroup, &ButtonGroup::playHandSendCard, this, &GamePanel::onUserPlayHand);
    connect(ui->butGroup, &ButtonGroup::passSendCard, this, &GamePanel::onUserPass);
    //处理玩家下注
    connect(ui->butGroup, &ButtonGroup::betPoint, this, [=](int bet){
        //通知玩家下注
        gc->getUserPlayer()->grabLordBet(bet);
        //下完注切换按钮组空页面
        ui->butGroup->selectPanel(ButtonGroup::Empty);
    });
}

void GamePanel::onUserPlayHand()
{
    qDebug()<<"onUserPlayerHand----------------gamepanel-262";
    // 判断游戏状态
    if(stat != GameControl::handSendCard)
    {
        return;
    }
    // 判断玩家是不是用户玩家
    if(gc->getCurrentPlayer() !=gc->getUserPlayer())
    {
    qDebug()<<"onUserPlayerHand---玩家是用户玩家-----------gamepanel-276";
        return;
    }
    // 判断要出的牌是否为空
    if(m_selectCards.isEmpty())
    {
    qDebug()<<"onUserPlayerHand--出的牌是为空------------gamepanel-276";
        return;
    }
    // 得到要打出的牌的牌型
    qDebug()<<"onUserPlayerHand---得到要打出的牌的牌型-------------gamepanel-279";
    Cards cs;
    for(auto it = m_selectCards.begin(); it != m_selectCards.end(); ++it)
    {
        Card card = (*it)->getCard();
        cs.add(card);
    }
    PlayHand hand(cs);
    PlayHand::HandType type = hand.getHandType();
    if(type == PlayHand::Hand_Unknown)
    {
        qDebug()<<"onUserPlayerHand---出的牌型未知-----------gamepanel-292";
        return;
    }
    // 判断当前玩家的牌能不能压住上一家的牌
    if(gc->getPendPlayer() != gc->getUserPlayer())
    {
        Cards cards = gc->getSendHandCards();
        if(!hand.canBeat(PlayHand(cards)))
        {
            qDebug()<<"onUserPlayerHand---前玩家的牌不能压住上一家的牌-----------gamepanel-279";
            return;
        }
    }
    m_countDown->stopCountDown();
    // 通过玩家对象出牌
    gc->getUserPlayer()->playerOutCard(cs);
    // 清空容器
    m_selectCards.clear();
}

void GamePanel::startDispatchCard()
{
        // 重置每张卡牌的属性
    for(auto &vulue:CardPenalMap)
    {
        vulue->setSeleted(false);
        vulue->setShowFront(true);
        vulue->hide();

    }
        //隐藏3张底牌
    for(int i =0;i<m_last3Card.size();i++)
    {
        m_last3Card[i]->hide();
    }
        //重置玩家上一局遗留的上下文信息
    int index = PlayerList.indexOf(gc->getUserPlayer());
    for(int i = 0;i<PlayerList.size();i++)
    {
        m_contextMap[PlayerList[i]].lastCards.clear();
        m_contextMap[PlayerList[i]].info->hide();
        m_contextMap[PlayerList[i]].roleImg->hide();
        m_contextMap[PlayerList[i]].isFrontSide = index==i ? true:false;
    }
        //重置玩家所有卡牌数据
    gc->resetCardDate();
        //显示专门用来显示的底牌
    m_baseCard->show();
        //隐藏按钮组面板
    ui->butGroup->selectPanel(ButtonGroup::Empty);
        //启动定时器
    m_timer->start(10);
        //播放背景音乐
     m_bgm->playAssistMusic(BGMControl::Dispatch);
}
void GamePanel::disposeCard(Player *player, const Cards &cards)
{
    Cards& myCard = const_cast<Cards&>(cards);
    //将扑克牌转化为降序排列
    QCardList list = myCard.toSortList();
    for(int i=0; i<list.size(); ++i)
    {
        CardPanel* panel = CardPenalMap[list.at(i)];
        panel->setOwner(player);
    }

    // 更新扑克牌在窗口中的显示
    updatePlayerCards(player);
}

void GamePanel::updatePlayerCards(Player *player)
{
    Cards cards = player->getCards();
    QCardList list = cards.toSortList();

    m_cardsRect = QRect();
    m_userCards.clear();
    // 取出展示扑克牌的区域
    int cardSpace = 20;
    QRect cardsRect = m_contextMap[player].cardRect;
    for(int i=0; i<list.size(); ++i)
    {
        CardPanel* panel = CardPenalMap[list.at(i)];
        panel->show();
        panel->raise();//堆叠显示
        panel->setShowFront(m_contextMap[player].isFrontSide);

        // 水平 or 垂直显示
        if(m_contextMap[player].align == Horizontal)
        {
            //获得存放区域和卡牌存放位置的间隙
            int leftX = cardsRect.left() + (cardsRect.width() - (list.size() - 1) * cardSpace - panel->width()) / 2;
            int topY = cardsRect.top() + (cardsRect.height() - gc->m_cardSize.height()) / 2;
            //选中弹出
            if(panel->isSelected())
            {
                topY -= 10;
            }
            //卡牌层叠移动
            panel->move(leftX + cardSpace * i, topY);
            m_cardsRect = QRect(leftX, topY,cardSpace * i + gc->m_cardSize.width(), gc->m_cardSize.height());
            int curWidth = 0;
            //最后一张卡牌全部展示
            if(list.size() - 1 == i)
            {
                curWidth = gc->m_cardSize.width();
            }
            else
            {
                curWidth = cardSpace;
            }
            QRect cardRect(leftX + cardSpace * i, topY, curWidth, gc->m_cardSize.height());
            //插入到用户手里卡牌面板map中，方便框选使用
            m_userCards.insert(panel, cardRect);
        }
        else
        {
            int leftX = cardsRect.left() + (cardsRect.width() - gc->m_cardSize.width()) / 2;
            int topY = cardsRect.top() + (cardsRect.height() - (list.size() - 1) * cardSpace - panel->height()) / 2;
            panel->move(leftX, topY + i * cardSpace);
        }
    }

    // 显示玩家打出的牌 到出牌区域
    // 得到当前玩家的出牌区域以及本轮打出的牌
    QRect playCardRect = m_contextMap[player].playHandRect;
    Cards lastCards = m_contextMap[player].lastCards;
    if(!lastCards.isEmpty())
    {
        //每张扑克牌显示时的间隙
        int playSpacing = 24;
        QCardList lastCardList = lastCards.toSortList();
        QCardList::ConstIterator itplayed = lastCardList.constBegin();
        for(int i=0; itplayed != lastCardList.constEnd(); ++itplayed, i++)
        {
            CardPanel* panel = CardPenalMap[*itplayed];
            panel->setShowFront(true);
            panel->raise();
            // 将打出的牌移动到出牌区域中心位置
            if(m_contextMap[player].align == Horizontal)
            {
                //中心位置的left（相对整个主窗口）+出牌区域宽度-（（size-1 * 间隙 + 扑克牌的宽度）/2）
                int leftBase = playCardRect.left() +
                               (playCardRect.width() - (lastCardList.size() - 1) * playSpacing - panel->width()) / 2;
                int top = playCardRect.top() + (playCardRect.height() - panel->height()) /2 ;
                panel->move(leftBase + i * playSpacing, top);
            }
            else
            {
                int left = playCardRect.left() + (playCardRect.width() - panel->width()) / 2;
                int top = playCardRect.top() ;
                panel->move(left, top + i * playSpacing);
            }
            panel->show();
        }
    }
}

void GamePanel::onPlayerStatusChanged(Player *player, GameControl::PlayStatus status)
{
    switch (status)
    {
        //思考是否抢地主（押注 012 3谁最高谁就是地主）
    case GameControl::thinkCallLords:
        //如果当前玩家是用户玩家显示按钮组
        if(player ==gc ->getUserPlayer())
        {
            ui->butGroup->selectPanel(ButtonGroup::CallLord, gc->getPlayerMaxBet());
        }
        break;
    case GameControl::thinkHandSendCard:
         //1. 隐藏上一轮打出的牌
        hidePlayerDropCards(player);
        if(player == gc->getUserPlayer())
        {
            // 取出出牌玩家的对象
            Player* pendplayer = gc->getPendPlayer();
            if(pendplayer == nullptr)
                qDebug()<<"onplayerStatuschange -------------------gamepanel---376";
            //当上一次出牌玩家为自己或者上次没人出牌也就是当前玩家是第一个出牌的时 切换到出牌面板
            if(pendplayer == gc->getUserPlayer() || pendplayer == nullptr)
            {
                ui->butGroup->selectPanel(ButtonGroup::PlayCard);
            }
            else
            {
                qDebug()<<"onplayerStatuschange -------------------gamepanel---380";
                ui->butGroup->selectPanel(ButtonGroup::PassOrPlay);
            }
        }
        else
        {
            ui->butGroup->selectPanel(ButtonGroup::Empty);
        }
        break;
    case GameControl::win:
        qDebug()<<"onplayerStatuschange ----win---------------gamepanel---488";
        m_bgm->stopBGM();
        //赢了将机器人玩家的牌给反过来展示出来
        m_contextMap[gc->getLeftRobt()].isFrontSide = true;
        m_contextMap[gc->getRightRobt()].isFrontSide = true;
        //刷新机器人卡牌面板正反面
        updatePlayerCards(gc->getLeftRobt());
        updatePlayerCards(gc->getRightRobt());
        // 更新玩家的得分
        updatePlayerScore();
        gc->setCurrentPlayer(player);
        showEndingScorePanel();
        break;
    default:
        break;
    }

}
void GamePanel::showAnimation(AnimationType type, int bet)
{

    switch(type)
    {
    case AnimationType::LianDui:
    case AnimationType::ShunZi:
        m_animation->setFixedSize(250, 150);
        m_animation->move((width()-m_animation->width())/2, 200);
        m_animation->showSequence((AnimationWindow::Type)type);
        break;
    case AnimationType::Plane:
        m_animation->setFixedSize(800, 75);
        m_animation->move((width()-m_animation->width())/2, 200);
        m_animation->showPlane();
        break;
    case AnimationType::Bomb:
        m_animation->setFixedSize(180, 200);
        m_animation->move((width()-m_animation->width())/2, (height() - m_animation->height()) / 2 - 70);
        m_animation->showBomb();
        break;
    case AnimationType::JokerBomb:
        m_animation->setFixedSize(250, 200);
        m_animation->move((width()-m_animation->width())/2, (height() - m_animation->height()) / 2 - 70);
        m_animation->showJokerBomb();
        break;
    case AnimationType::Bet:
        m_animation->setFixedSize(160, 98);
        m_animation->move((width()-m_animation->width())/2, (height()-m_animation->height())/2-140);
        m_animation->showBetScore(bet);
        break;
    }
    m_animation->show();
}

void GamePanel::onDisposePlayHand(Player *player, const Cards &cards)
{
    // 存储玩家打出的牌
    auto it = m_contextMap.find(player);
    it->lastCards = cards;
    // 2. 根据牌型播放游戏特效
    Cards& myCards = const_cast<Cards&>(cards);
    PlayHand hand(myCards);
    PlayHand::HandType type = hand.getHandType();
    if(type == PlayHand::Hand_Plane ||
        type == PlayHand::Hand_Plane_Two_Pair ||
        type == PlayHand::Hand_Plane_Two_Single)
    {
        showAnimation(Plane);
    }
    else if(type == PlayHand::Hand_Seq_Pair)
    {
        showAnimation(LianDui);
    }
    else if(type == PlayHand::Hand_Seq_Single)
    {
        showAnimation(ShunZi);
    }
    else if(type == PlayHand::Hand_Bomb)
    {
        showAnimation(Bomb);
    }
    else if(type == PlayHand::Hand_Bomb_Jokers)
    {
        showAnimation(JokerBomb);
    }
    // 如果玩家打出的是空牌(不出牌), 显示提示信息
    if(cards.isEmpty())
    {
        it->info->setPixmap(QPixmap(":/images/pass.png"));
        it->info->show();
        m_bgm->playPassMusic((BGMControl::RoleSex)player->getSex());
    }
    else
    {
        if(gc->getPendPlayer() == player || gc->getPendPlayer() == nullptr)
        {
            m_bgm->playCardMusic(cards, true, (BGMControl::RoleSex)player->getSex());
        }
        else
        {
            m_bgm->playCardMusic(cards, false, (BGMControl::RoleSex)player->getSex());
        }
    }
    // 3. 更新玩家剩余的牌
    updatePlayerCards(player);
    // 4. 播放提示音乐
    // 判断玩家剩余的牌的数量
    if(player->getCards().count() == 2)
    {
        m_bgm->playLastMusic(BGMControl::Last2, (BGMControl::RoleSex)player->getSex());
    }
    else if(player->getCards().count() == 1)
    {
        m_bgm->playLastMusic(BGMControl::Last1, (BGMControl::RoleSex)player->getSex());
    }

}

void GamePanel::hidePlayerDropCards(Player *player)
{
    auto it = m_contextMap.find(player);
    //判断迭代器是否有效 end（）最后一个元素加1
    if(it != m_contextMap.end())
    {
        //如果上次没出牌隐藏 不出图片 上一次的提示动画
        if(it->lastCards.isEmpty())
        {
            it->info->hide();
        }
        else
        {
            // Cards --> Card
            //隐藏上次打出的牌
            QCardList list = it->lastCards.toSortList();
            for(auto last = list.begin(); last != list.end(); ++last)
            {
                CardPenalMap[*last]->hide();
            }
        }
        //隐藏完删除上一次最后打出的牌，防止下一次出牌还会一直显示
        it->lastCards.clear();
    }
}
//flag判断是不是第一个抢地主的玩家
void GamePanel::onGrabLordBet(Player *player, int bet, bool flag)
{
    // 显示抢地主的信息提示
    PlayerContext context = m_contextMap[player];
    //如果下注为0 更新不抢提示
    if(bet == 0)
    {
        context.info->setPixmap(QPixmap(":/images/buqinag.png"));
    }
    else
    {
        if(flag)
        {
            context.info->setPixmap(QPixmap(":/images/jiaodizhu.png"));
        }
        else
        {
            context.info->setPixmap(QPixmap(":/images/qiangdizhu.png"));
        }
        // 显示叫地主的分数
        showAnimation(Bet, bet);
    }
    context.info->show();

    // 播放分数的背景音乐
    m_bgm->playerRobLordMusic(bet, (BGMControl::RoleSex)player->getSex(), flag);
}
void GamePanel::onDispatchCard()
{
    //
    static int currMovePos = 0;
    Player*curPlayer = gc->getCurrentPlayer();
    if(currMovePos >= 100)
    {
        Card card=gc->takeOneCard();
        curPlayer->storeCard(card);
        Cards cs(card);
       // disposeCard(curPlayer, cs);
        //切换当前玩家
        gc->setCurrentPlayer(curPlayer->getNextPlayer());
        currMovePos =0;
        //发牌动画
        cardMoveStep(curPlayer,currMovePos);
        //判断牌是否发完了
        if(gc->getSurplusThreeCards().count() == 3)
        {
            //终止定时器
            m_timer->stop();
            //移动牌和中央底牌都隐藏才行
//            m_baseCard->hide();
//            m_moveCard->hide();
            //发牌完成切换游戏状态到叫地主状态
            gameStatusPrecess(GameControl::callLords);
            //关闭音乐
            return;
        }
    }
    //移动扑克
    cardMoveStep(curPlayer,currMovePos);
    //每次移动几个步长
    currMovePos+=15;
}
//curPos 每一次走到步数
void GamePanel::cardMoveStep(Player *player, int curPos)
{
    // 得到每个玩家的扑克牌展示区域
    QRect cardRect = m_contextMap[player].cardRect;
    // 每个玩家的单元步长 分成100份
    const int unit[] = {
        (m_baseCardPos.x() - cardRect.right()) / 100,
        (cardRect.left() - m_baseCardPos.x()) / 100,
        (cardRect.top() - m_baseCardPos.y()) / 100
    };
    // 每次窗口移动的时候每个玩家对应的牌的时时坐标位置
    QPoint pos[] =
        {
        //左侧机器人从坐标点往左减小
            QPoint(m_baseCardPos.x() - curPos * unit[0], m_baseCardPos.y()),
        //右侧机器人从坐标点往右增加
            QPoint(m_baseCardPos.x() + curPos * unit[1], m_baseCardPos.y()),
        //user 向下增加
            QPoint(m_baseCardPos.x(), m_baseCardPos.y() + curPos * unit[2]),
        };

    // 移动扑克牌窗口
    int index = PlayerList.indexOf(player);
    m_moveCard->move(pos[index]);

    // 临界状态处理
    if(curPos == 0)
    {
        m_moveCard->show();
    }
    if(curPos == 100)
    {
        m_moveCard->hide();
    }
}
void GamePanel::showEndingScorePanel()
{
    qDebug()<<"showEnscorePanel -------------- gamepanel-730";
    bool islord = gc->getUserPlayer()->getRole() == Player::lord ? true : false;
    bool isWin = gc->getUserPlayer()->getIsWin();
    EndingPanel* panel = new EndingPanel(islord, isWin, this);
    panel->show();
    //移动到整个gampenal的上方位置也就是动态下缀的起始位置
    panel->move((width() - panel->width()) / 2, -panel->height());
    panel->setPlayerScore(gc->getLeftRobt()->getScore(),
                          gc->getRightRobt()->getScore(),
                          gc->getUserPlayer()->getScore());
    if(isWin)
    {
        m_bgm->playEndingMusic(true);
    }
    else
    {
        m_bgm->playEndingMusic(false);
    }
    //设置弹跳动画
    QPropertyAnimation *animation = new QPropertyAnimation(panel, "geometry", this);
    // 动画持续的时间
    animation->setDuration(1500);   // 1.5s
    // 设置窗口的起始位置和终止位置
    animation->setStartValue(QRect(panel->x(), panel->y(), panel->width(), panel->height()));
    animation->setEndValue(QRect((width() - panel->width()) / 2, (height() - panel->height()) / 2,
                                 panel->width(), panel->height()));
    // 设置窗口的运动曲线
    animation->setEasingCurve(QEasingCurve(QEasingCurve::OutBounce));
    // 播放动画效果
    animation->start();

    // 处理窗口信号
    connect(panel, &EndingPanel::continueGame, this, [=]()
            {
                panel->close();
                panel->deleteLater();
                animation->deleteLater();
                ui->butGroup->selectPanel(ButtonGroup::Empty);
                qDebug()<<"游状态改变 sendCard------ showEndscorePanel -------------- gamepanel-769";
                gameStatusPrecess(GameControl::sendCard);
                m_bgm->startBGM(80);
            });
}

void GamePanel::mouseMoveEvent(QMouseEvent *ev)
{
    //判断是不是左键在一直按做框选
    Q_UNUSED(ev)
    if(ev->buttons() & Qt::LeftButton)
    {
        QPoint pt = ev->pos();
        if(!m_cardsRect.contains(pt))
        {
            m_curSelCard = nullptr;
        }
        else
        {
            //遍历每个卡牌的存放坐标 判断移动到的当前的鼠标坐标是不是经过了卡片的起始坐标
            QList<CardPanel*> list = m_userCards.keys();
            for(int i=0; i<list.size(); ++i)
            {
                CardPanel* panel = list.at(i);
                if(m_userCards[panel].contains(pt) &&m_curSelCard != panel)
                {
                    // 模拟点击这张扑克牌
                    panel->clicked();
                    //只要经过了就把当前选择坐标设置为这个卡牌的起始坐标
                    m_curSelCard = panel;
                }
            }
        }
    }
}

void GamePanel::onUserPass()
{
    m_countDown->stopCountDown();
    // 判断是不是用户玩家
    qDebug()<<"------- onUserPass----gamepanel-761";
    Player* curPlayer = gc->getCurrentPlayer();
    Player* userPlayer = gc->getUserPlayer();
    if(curPlayer != userPlayer)
    {
        qDebug()<<"当前玩家是用户玩家 ------- onUserPass----gamepanel-761";
        return;
    }
    // 判断当前用户玩家是不是上一次出牌的玩家(可以不处理)
    Player* pendPlayer = gc->getPendPlayer();
    if(pendPlayer == userPlayer || pendPlayer == nullptr)
    {
        qDebug()<<"当前玩家是上一次出牌的玩家或者是第一次出牌的 ------- onUserPass----gamepanel-773";
        return;
    }
    // 打出一个空的Cards对象
    Cards empty;
    userPlayer->playerOutCard(empty);
    // 清空用户选择的牌(玩家可能选择了一些牌, 但是没有打出去)
    for(auto it = m_selectCards.begin(); it != m_selectCards.end(); ++it)
    {
        (*it)->setSeleted(false);
    }
    m_selectCards.clear();
    qDebug()<<"更新出牌区域的牌 ------- onUserPass----gamepanel-785";
    // 更新玩家待出牌区域的牌
    updatePlayerCards(userPlayer);
} 

void GamePanel::initGameScene()
{
    //3张底牌隐藏
    for(int i=0; i<3; ++i)
    {
        CardPanel* panel = new CardPanel(this);
        panel->setImage(gc->m_cardBackImg, gc->m_cardBackImg);
        m_last3Card.push_back(panel);
        panel->hide();
    }
    //游戏开始发牌区显示的一张牌
    m_baseCard = new CardPanel(this);
    m_baseCard->setImage(gc->m_cardBackImg, gc->m_cardBackImg);
    //发牌过程中移动的扑克牌
    m_moveCard = new CardPanel(this);
    m_moveCard->setImage(gc->m_cardBackImg, gc->m_cardBackImg);

    //移动各个扑克牌
    m_baseCardPos = QPoint((width() - gc->m_cardSize.width()) / 2,
                           height() / 2 - 100);
    m_baseCard->move(m_baseCardPos);
    m_moveCard->move(m_baseCardPos);

    //移动3张底牌到上面3个位置
    int base = (width() - 3 * gc->m_cardSize.width() - 2 * 10) / 2;
    for(int i=0; i<3; ++i)
    {
        m_last3Card[i]->move(base + (gc->m_cardSize.width() + 10) * i, 20);
        m_last3Card[i]->setShowFront(true);
    }

}
QPixmap GamePanel::loadRoleImage(Player::Sex sex, Player::Derict direct, Player::Role role)
{
    // 找图片
    QVector<QString> lordMan;
    QVector<QString> lordWoman;
    QVector<QString> farmerMan;
    QVector<QString> farmerWoman;
    lordMan << ":/images/lord_man_1.png" << ":/images/lord_man_2.png";
    lordWoman << ":/images/lord_woman_1.png" << ":/images/lord_woman_2.png";
    farmerMan << ":/images/farmer_man_1.png" << ":/images/farmer_man_2.png";
    farmerWoman << ":/images/farmer_woman_1.png" << ":/images/farmer_woman_2.png";

    // 各个角色两张图片随机加载  QImage
    QImage image;
    int random = QRandomGenerator::global()->bounded(2);
    if(sex == Player::man && role == Player::lord)
    {
        image.load(lordMan.at(random));
    }
    else if(sex == Player::man && role == Player::farmer)
    {
        image.load(farmerMan.at(random));
    }
    else if(sex == Player::women && role == Player::lord)
    {
        image.load(lordWoman.at(random));
    }
    else if(sex == Player::women && role == Player::farmer)
    {
        image.load(farmerWoman.at(random));
    }

    //将加载后图片转化为pixmap
    QPixmap pixmap;
    //因为图像都是向右的因此需要镜像翻转
    if(direct == Player::left)
    {
        pixmap = QPixmap::fromImage(image);
    }
    else
    {
        pixmap = QPixmap::fromImage(image.mirrored(true, false));
    }
    return pixmap;
}

void GamePanel::gameStatusPrecess(GameControl::GameStatus status)
{
    //记录游戏状态
    stat = status;
 // 处理游戏状态
    switch(status)
    {
    case GameControl::sendCard:
        startDispatchCard();
        break;
    case GameControl::callLords:
    {
//        // 取出底牌数据
        QCardList last3Card = gc->getSurplusThreeCards().toSortList();
        // 给底牌窗口设置图片
        for(int i=0; i<last3Card.size(); ++i)
        {
             QPixmap front = CardPenalMap[last3Card.at(i)]->getImage();
             qDebug()<<i<<"卡牌点数为"<<last3Card.at(i).Point;
             m_last3Card[i]->setImage(front,gc->m_cardBackImg);
             qDebug()<<i<<"is front"<<m_last3Card[i]->isShowFront();
             m_last3Card[i]->hide();
             //m_last3Card[i]->show();
        }
//        // 开始叫地主
        gc->statrCallLord();
        break;
    }
    case GameControl::handSendCard:
        // 隐藏发牌区的底牌和移动的牌
        m_baseCard->hide();
        m_moveCard->hide();
        // 显示留给地主的三张底牌
        for(int i=0; i<m_last3Card.size(); ++i)
        {
            m_last3Card.at(i)->show();
        }
        for(int i=0; i<PlayerList.size(); ++i)
        {
            PlayerContext &context = m_contextMap[PlayerList.at(i)];
            // 隐藏各个玩家抢地主过程中的提示信息
            context.info->hide();
            // 显示各个玩家的头像
            Player* player = PlayerList.at(i);
            QPixmap pixmap = loadRoleImage(player->getSex(), player->getDirection(), player->getRole());
            context.roleImg->setPixmap(pixmap);
            context.roleImg->show();
        }
    break;
    default:
    break;
    }
}
void GamePanel::initCountDown()
{
    m_countDown = new CountDown(this);
    m_countDown->move((width() - m_countDown->width()) / 2, (height() - m_countDown->height()) / 2 + 30);
    connect(m_countDown, &CountDown::notMuchTime, this, [=](){
        // 播放提示音乐
        m_bgm->playAssistMusic(BGMControl::Alert);
    });
   //时间结束默认不出
    connect(m_countDown, &CountDown::timeout, this, &GamePanel::onUserPass);
    UserPlayer* userPlayer = gc->getUserPlayer();
    connect(userPlayer, &UserPlayer::startCountDown, this, [=](){
        if(gc->getPendPlayer() != userPlayer && gc->getPendPlayer() != nullptr)
        {
            m_countDown->showCountDown();
        }
    });
}
void GamePanel::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)
    QPainter p(this);
    p.drawPixmap(rect(), m_bkImage);
}

