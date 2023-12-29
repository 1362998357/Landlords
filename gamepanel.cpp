#include "gamepanel.h"
#include "cardpanel.h"
#include "ui_gamepanel.h"
#include <QRandomGenerator>
#include <QPainter>
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
    initTimeEndCount();
    //定时初始化
    m_timer = new QTimer(this);
    //定时器链接发牌处理函数
    connect(m_timer,&QTimer::timeout,this, &GamePanel::onDispatchCard);
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
    //当游戏玩家状态改变后发出信号调用游戏面板的改变函数
    connect(gc,&GameControl::playerStatusChanged,this,&GamePanel::onPlayerStatusChanged);
    connect(gc,&GameControl::notifyGrabLordBet,this,&GamePanel::onGrabLordBet);
    connect(gc, &GameControl::gameStatusChanged, this, &GamePanel::gameStatusPrecess);
    //connect(gc, &GameControl::notifyPlayHand, this, &GamePanel::onDisposePlayHand);

//    connect(gc->getLeftRobt(), &Player::notifyPickCards, this, &GamePanel::disposeCard);
//    connect(gc->getRightRobt(), &Player::notifyPickCards, this, &GamePanel::disposeCard);
//    connect(gc->getUserPlayer(), &Player::notifyPickCards, this, &GamePanel::disposeCard);
}

void GamePanel::updatePlayerScore()
{
    int left = PlayerList[0]->getScore();
    int right = PlayerList[1]->getScore();
    int user = PlayerList[2]->getScore();
    ui->scorePanelsEff->setScores(left,right,user);
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

    c.setPoint(Card::Card_BJ);
    gc->cropImage(pixmap, panel2,gc->m_cardSize.width(), 4*gc->m_cardSize.height(), c);
    this->CardPenalMap.insert(c,panel2);
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
    connect(ui->butGroup, &ButtonGroup::startGame, this, [=](){
        // 界面的初始化
        ui->butGroup->selectPanel(ButtonGroup::Empty);
        gc->clearPlayerScore();
        updatePlayerScore();
        // 修改游戏状态 -> 发牌
        gameStatusPrecess(GameControl::sendCard);
        // 播放背景音乐
       // bgm->startBGM(80);
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

void GamePanel::initTimeEndCount()
{

}

void GamePanel::onUserPlayHand()
{

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
        panel->raise();
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
            //插入到用户手里卡牌面板map中，方便后续使用
            m_userCards.insert(panel, cardRect);
        }
        else
        {
            int leftX = cardsRect.left() + (cardsRect.width() - gc->m_cardSize.width()) / 2;
            int topY = cardsRect.top() + (cardsRect.height() - (list.size() - 1) * cardSpace - panel->height()) / 2;
            panel->move(leftX, topY + i * cardSpace);
        }
    }

//    // 显示玩家打出的牌
//    // 得到当前玩家的出牌区域以及本轮打出的牌
//    QRect playCardRect = m_contextMap[player].playHandRect;
//    Cards lastCards = m_contextMap[player].lastCards;
//    if(!lastCards.isEmpty())
//    {
//        int playSpacing = 24;
//        CardList lastCardList = lastCards.toCardList();
//        CardList::ConstIterator itplayed = lastCardList.constBegin();
//        for(int i=0; itplayed != lastCardList.constEnd(); ++itplayed, i++)
//        {
//            CardPanel* panel = m_cardMap[*itplayed];
//            panel->setFrontSide(true);
//            panel->raise();
//            // 将打出的牌移动到出牌区域
//            if(m_contextMap[player].align == Horizontal)
//            {
//                int leftBase = playCardRect.left() +
//                               (playCardRect.width() - (lastCardList.size() - 1) * playSpacing - panel->width()) / 2;
//                int top = playCardRect.top() + (playCardRect.height() - panel->height()) /2 ;
//                panel->move(leftBase + i * playSpacing, top);
//            }
//            else
//            {
//                int left = playCardRect.left() + (playCardRect.width() - panel->width()) / 2;
//                int top = playCardRect.top() ;
//                panel->move(left, top + i * playSpacing);
//            }
//            panel->show();
//        }
    //    }
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
        // 1. 隐藏上一轮打出的牌
//        hidePlayerDropCards(player);
//        if(player == gc->getuserplayer())
//        {
//            // 取出出牌玩家的对象
//            player* pendplayer = gc->getuuuplayer();
//            if(pendplayer == gc->getuserplayer() || pendplayer == nullptr)
//            {
//                ui->butgroup->selectpanel(buttongroup::playcard);
//            }
//            else
//            {
//                ui->butgroup->selectpanel(buttongroup::passorplay);
//            }
//        }
//        else
//        {
//            ui->butgroup->selectpanel(buttongroup::empty);
//        }
        break;
    case GameControl::win:
//        m_bgm->stopBGM();
//        m_contextMap[gc->getLeftRobt()].isFrontSide = true;
//        m_contextMap[gc->getRightRobt()].isFrontSide = true;
//        updatePlayerCards(gc->getLeftRobt());
//        updatePlayerCards(gc->getRightRobt());
//        // 更新玩家的得分
//        updatePlayerScore();
//        gc->setCurrentPlayer(player);
//        showEndingScorePanel();
        break;
    default:
        break;
    }

}
void GamePanel::showAnimation(AnimationType type, int bet)
{

//    switch(type)
//    {
//    case AnimationType::LianDui:
//    case AnimationType::ShunZi:
//        m_animation->setFixedSize(250, 150);
//        m_animation->move((width()-m_animation->width())/2, 200);
//        m_animation->showSequence((AnimationWindow::Type)type);
//        break;
//    case AnimationType::Plane:
//        m_animation->setFixedSize(800, 75);
//        m_animation->move((width()-m_animation->width())/2, 200);
//        m_animation->showPlane();
//        break;
//    case AnimationType::Bomb:
//        m_animation->setFixedSize(180, 200);
//        m_animation->move((width()-m_animation->width())/2, (height() - m_animation->height()) / 2 - 70);
//        m_animation->showBomb();
//        break;
//    case AnimationType::JokerBomb:
//        m_animation->setFixedSize(250, 200);
//        m_animation->move((width()-m_animation->width())/2, (height() - m_animation->height()) / 2 - 70);
//        m_animation->showJokerBomb();
//        break;
//    case AnimationType::Bet:
//        m_animation->setFixedSize(160, 98);
//        m_animation->move((width()-m_animation->width())/2, (height()-m_animation->height())/2-140);
//        m_animation->showBetScore(bet);
//        break;
//    }
//    m_animation->show();
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
        //showAnimation(Bet, bet);
    }
    context.info->show();

    // 播放分数的背景音乐
    //m_bgm->playerRobLordMusic(bet, (BGMControl::RoleSex)player->getSex(), flag);
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
        disposeCard(curPlayer, cs);
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
            //切换游戏状态到叫地主状态
            gameStatusPrecess(GameControl::callLords);
            //关闭音乐
            return;
        }
    }
    //移动扑克
    cardMoveStep(curPlayer,currMovePos);
    currMovePos+=8;
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

void GamePanel::mouseMoveEvent(QMouseEvent *ev)
{

}

void GamePanel::onUserPass()
{

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
    }

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
             m_last3Card[i]->setImage(front, m_bkImage);
             m_last3Card[i]->hide();
        }
//        // 开始叫地主
        gc->statrCallLord();
        break;
    }
    case GameControl::handSendCard:
    break;
    default:
    break;
    }
}

void GamePanel::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)
    QPainter p(this);
    p.drawPixmap(rect(), m_bkImage);
}

