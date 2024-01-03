#include "gamecontrol.h"
#include <QDebug>
#include "playhand.h"
#include <QTimer>
GameControl::GameControl(QObject *parent)
    : QObject{parent}
{

}

void GameControl::playerInint()
{
    leftRobot=new Robot("机器人A",this);
    rightRobot=new Robot("机器人B",this);
    userPlayer=new UserPlayer("我自己",this);
    //头像的显示方位
    leftRobot->setDirection(Player::left);
    rightRobot->setDirection(Player::right);
    userPlayer->setDirection(Player::right);

    //sex
    Player::Sex sex;
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    leftRobot->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    rightRobot->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    userPlayer->setSex(sex);
    //出牌顺序
    userPlayer->setNextPlayer(rightRobot);
    userPlayer->setPrePlayer(leftRobot);

    leftRobot->setNextPlayer(userPlayer);
    leftRobot->setPrePlayer(rightRobot);

    rightRobot->setNextPlayer(leftRobot);
    rightRobot->setPrePlayer(userPlayer);

    //指定当前玩家
    currentPlayer = userPlayer;
   //处理玩家下注
    connect(userPlayer,&UserPlayer::notifyGrabLordBet,this,&GameControl::onGrabBet);
    connect(leftRobot, &Robot::notifyGrabLordBet, this, &GameControl::onGrabBet);
    connect(rightRobot, &Robot::notifyGrabLordBet, this, &GameControl::onGrabBet);

    // 传递出牌玩家对象和玩家打出的牌

    connect(this, &GameControl::pendingInfo, leftRobot, &Robot::storeSendCardsInfor);
    connect(this, &GameControl::pendingInfo, rightRobot, &Robot::storeSendCardsInfor);
    connect(this, &GameControl::pendingInfo, userPlayer, &Robot::storeSendCardsInfor);

    // 处理玩家出牌
    connect(leftRobot, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);
    connect(rightRobot, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);
    connect(userPlayer, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);

}

Robot *GameControl::getLeftRobt()
{
    return leftRobot;
}

Robot *GameControl::getRightRobt()
{
    return rightRobot;
}

UserPlayer *GameControl::getUserPlayer()
{
    return userPlayer;
}

void GameControl::setCurrentPlayer(Player *player)
{
    currentPlayer = player;
}

Player *GameControl::getCurrentPlayer()
{
    return currentPlayer;
}

Cards GameControl::getSendHandCards()
{
    return sendHandPlayerCards;
}

Player *GameControl::getPendPlayer()
{
    return sendHandPlayer;
}
Card GameControl::takeOneCard()
{
    return allCards.randSendCard();
}

Cards GameControl::getSurplusThreeCards()
{
    //因为时随机发送的因此全部给玩家发完剩余的就是3张底牌
    return allCards;
}

void GameControl::initCards()
{
    for(int i =Card::PointBegin+1;i!=Card::Card_SJ;i++)
    {
        for(int j =Card::SuitBegin+1;j!=Card::SuitEnd;j++)
        {
            allCards.add(Card((Card::CardPoint)i,(Card::CardSuit)j));
        }
    }
    allCards.add(Card(Card::Card_SJ,Card::SuitBegin));
    allCards.add(Card(Card::Card_BJ,Card::SuitBegin));

}

void GameControl::resetCardDate()
{
    initCards();
    //清空所有人手里的牌
    leftRobot->clearMyCards();
    rightRobot->clearMyCards();
    userPlayer->clearMyCards();
    //初始化出牌玩家和牌
    this->sendHandPlayer=nullptr;
    sendHandPlayerCards.clear();
}

void GameControl::statrCallLord()
{
    //开始准备叫地主 会调用3个类的槽函数 线程就是在这个函数里面创建
    currentPlayer->prepareCallLord();
    emit playerStatusChanged(currentPlayer,thinkCallLords);
}

void GameControl::becomeLord(Player*player,int&bet)
{
    currentPlayer->setRole(Player::lord);
    currentPlayer->getNextPlayer()->setRole(Player::farmer);
    currentPlayer->getPrePlayer()->setRole(Player::farmer);
    currentPlayer = player;
    //地主获得3张底牌
    qDebug()<<"MAx Point"<<allCards.getMaxPoint();
    currentPlayer->storeCards(allCards);

    //准备发牌
    QTimer::singleShot(1000, this, [=]()
                       {
        //切换状态
        emit gameStatusChanged(handSendCard);
        emit playerStatusChanged(player, thinkHandSendCard);
        currentPlayer->prepareSendCards();
    });
}

void GameControl::clearScore()
{
    userPlayer->setScore(0);
    leftRobot->setScore(0);
    rightRobot->setScore(0);
}

void GameControl::clearPlayerScore()
{

}
int GameControl::getPlayerMaxBet()
{
    return m_betRecord.bet;
}

void GameControl::onGrabBet(Player *player, int bet)
{
    // 1. 通知主界面玩家叫地主了(更新信息提示动画)
    //bet==0不抢
    if(bet == 0 || m_betRecord.bet >= bet)
    {
        //调用的是gamecontrl中的notifyGrabLordBet 在gamepanel类中的initbuttons中connect
        emit notifyGrabLordBet(player, 0, false);
    }
    else if(bet > 0 && m_betRecord.bet == 0)
    {
        // 标记是第一个抢地主的玩家更新提示动画显示叫地主
        emit notifyGrabLordBet(player, bet, true);
    }
    else
    {
        // 第2,3个抢地主的玩家 更新提示显示抢地主
        emit notifyGrabLordBet(player, bet, false);
    }

    qDebug() << "curent player name: " << player->getName() << ", 下注分数: "
             << bet<< ", m_betRecord.times: " << m_betRecord.times;


    // 2. 判断玩家下注是不是3分, 如果是抢地主结束
    if(bet == 3)
    {
        // 玩家成为地主
        becomeLord(player, bet);
        // 清空数据
        m_betRecord.reset();
        return;
    }
    // 3. 下注不够3分, 对玩家的分数进行比较, 分数高的是地主
    if(m_betRecord.bet < bet)
    {
        //更新最下注最大的玩家到信息中
        m_betRecord.bet = bet;
        m_betRecord.player = player;
    }
    m_betRecord.times ++;
    // 如果每个玩家都抢过一次地主, 抢地主结束
    if(m_betRecord.times == 3)
    {
        //记录的赌注为0
        if(m_betRecord.bet == 0)
        {
            emit gameStatusChanged(handSendCard);
        }
        else
        {
            //让人数最大的人成为地主
            becomeLord(m_betRecord.player, m_betRecord.bet);
        }
        m_betRecord.reset();
        return;
    }
    // 4. 切换玩家, 通知下一个玩家继续抢地主
    currentPlayer = player->getNextPlayer();
    // 发送信号给主界面, 告知当前状态为抢地主切换下一个玩家 每个玩家都要抢地主
    emit playerStatusChanged(currentPlayer, thinkCallLords);
    currentPlayer->prepareCallLord();
}

void GameControl::onPlayHand(Player *player, const Cards &card)
{
    //相关信号初始化在void GameControl::playerInint()

    // 1. 将玩家出牌的信号转发给主界面
    emit notifyPlayHand(player, card);
    // 2. 如果不是空牌, 给其他玩家发送信号, 保存出牌玩家对象和打出的牌
    if(!card.isEmpty())
    {
        qDebug()<<"出牌玩家不为空 ----------GameControl::onPlayHand--246";
        //保存出的牌 和出牌玩家
        sendHandPlayerCards = card;
        sendHandPlayer = player;
        //通知其他玩家 保存出牌玩家信息
        emit pendingInfo(player, card);
    }
    // 如果有炸弹, 低分翻倍
    Cards myCards = const_cast<Cards&>(card);
    PlayHand::HandType type = PlayHand(myCards).getHandType();
    if(type == PlayHand::Hand_Bomb || type == PlayHand::Hand_Bomb_Jokers)
    {
        m_curBet = m_curBet * 2;
    }

    // 3. 如果玩家的牌出完了, 计算本局游戏的总分
    if(player->getCards().isEmpty())
    {
        Player* prev = player->getPrePlayer();
        Player* next = player->getNextPlayer();
        //如果当前玩家是地主
        if(player->getRole() == Player::lord)
        {
            player->setScore(player->getScore() + 2 * m_curBet);
            prev->setScore(prev->getScore() - m_curBet);
            next->setScore(next->getScore() - m_curBet);
            player->setWin(true);
            prev->setWin(false);
            next->setWin(false);
        }
        else
        {
            player->setWin(true);
            player->setScore(player->getScore() + m_curBet);
            if(prev->getRole() == Player::lord)
            {
                prev->setScore(prev->getScore() - 2 * m_curBet);
                next->setScore(next->getScore() + m_curBet);
                prev->setWin(false);
                next->setWin(true);
            }
            else
            {
                next->setScore(next->getScore() - 2 * m_curBet);
                prev->setScore(prev->getScore() + m_curBet);
                next->setWin(false);
                prev->setWin(true);
            }

        }
        //void GamePanel::gameControlInit()
        emit playerStatusChanged(player, GameControl::win);
        return;
    }
    // 4. 牌没有出完, 切换下一个玩家继续出牌
    qDebug()<<"currentplayer :"<<currentPlayer->getName();
    currentPlayer = player->getNextPlayer();
    qDebug()<<"currentplayer :"<<currentPlayer->getName();
    currentPlayer->prepareSendCards();
    //发送改变玩家状态为考虑出牌状态 的信号
    emit playerStatusChanged(currentPlayer, GameControl::thinkHandSendCard);
}



void GameControl::cropImage(QPixmap &pix, CardPanel*panel,int x, int y, Card &c)
{
    QPixmap sub = pix.copy(x, y, m_cardSize.width(), m_cardSize.height());
    panel->setImage(sub, m_cardBackImg);
    panel->setCard(c);
    panel->hide();
}
