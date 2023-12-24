#include "gamecontrol.h"

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
    leftRobot->setDirection(Player::right);

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
    currentPlayer->prepareCallLord();
}

void GameControl::becomeLord(Player*player)
{
    currentPlayer->setRole(Player::lord);
    currentPlayer->getNextPlayer()->setRole(Player::farmer);
    currentPlayer->getPrePlayer()->setRole(Player::farmer);
    currentPlayer = player;
    //地主获得3张底牌
    currentPlayer->storeCards(allCards);
    //准备发牌
    currentPlayer->prepareSendCards();
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

void GameControl::cropImage(QPixmap &pix, CardPanel*panel,int x, int y, Card &c)
{
    QPixmap sub = pix.copy(x, y, m_cardSize.width(), m_cardSize.height());
    panel->setImage(sub, m_cardBackImg);
    panel->setCard(c);
    panel->hide();
}
