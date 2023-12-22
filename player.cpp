#include "player.h"

Player::Player(QObject *parent)
    : QObject{parent}
{

}

Player::Player(QString name, QObject *parent)
{
    this->name = name;
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

void Player::setIswin(bool isWin)
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
}

void Player::storeCards(Cards &cards)
{
    myCards.add(cards);
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

}

void Player::storeSendCardsInfor(Cards cards, Player *player)
{
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


