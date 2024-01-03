#include "player.h"
#include <QDebug>
Player::Player(QObject *parent)
    : QObject{parent}
{
    score = 0;
    isWin = false;
}

Player::Player(QString name, QObject *parent):Player(parent)
{
    this->name = name;
    this->justSeCaPlayer =nullptr;
}

void Player::setName(QString name)
{
    this->name = name;
}

QString Player::getName()
{
    return name;
}

void Player::setDirection(Derict derict)
{
    this->derict = derict;
}

Player::Derict Player::getDirection()
{
    return derict;
}

void Player::setRole(Role role)
{
    this->role = role;
}

Player::Role Player::getRole()
{
    return role;
}

void Player::setSex(Sex sex)
{
    this->sex = sex;
}

Player::Sex Player::getSex()
{
    return sex;
}

void Player::setTyped(Typed typed)
{
    this->typed = typed;
}

Player::Typed Player::getTyped()
{
    return typed;
}

void Player::setScore(int score)
{
    this->score = score;
}

int Player::getScore()
{
    return score;
}

void Player::setWin(bool isWin)
{
    this->isWin = isWin;
}

bool Player::getIsWin()
{
    return isWin;
}

void Player::storeCard(Card &card)
{
    myCards.add(card);
    Cards cs;
    cs.add(card);
    emit notifyPickCards(this, cs);
}

void Player::storeCards(Cards &cards)
{
    myCards.add(cards);
    //通知面板保存了3张底牌 保证面板将3张底牌及时更新卡牌的所有者
    emit notifyPickCards(this, cards);
}

Cards Player::getCards()
{
    return myCards;
}

void Player::clearMyCards()
{
    myCards.clear();
}

void Player::playerOutCard(Cards &cards)
{
    myCards.remove(cards);
    //通知已经出牌出牌
    emit notifyPlayHand(this, cards);
}

void Player::setPrePlayer(Player *player)
{
    prePlayer = player;
}

void Player::setNextPlayer(Player *player)
{
    nextPlayer = player;
}

Player *Player::getPrePlayer()
{
    return prePlayer;
}

Player *Player::getNextPlayer()
{
    return nextPlayer;
}

void Player::grabLordBet(int point)
{
        emit notifyGrabLordBet(this, point);
}

void Player::storeSendCardsInfor(Player* player, const Cards& cards)
{
        qDebug()<<"player store name"<<getName()<<"----------------";
        qDebug()<<"player store name"<<player->getName()<<"----------------";
        qDebug()<<"player store card "<<cards.count()<<"----------------";
    justSendCards = cards;
    justSeCaPlayer = player;
}

Cards Player::getJustSendCards()
{
    return justSendCards;
}

Player *Player::getJustSendPlayer()
{
    return justSeCaPlayer;
}

void Player::prepareCallLord()
{

}

void Player::prepareSendCards()
{

}

void Player::thinkCallLords()
{
    qDebug()<<"Player thinkCallLord";
}

void Player::thinkPlayHand()
{

}


