#include "userplayer.h"

UserPlayer::UserPlayer(QString name, QObject *parent)
    : Player{parent}
{
    this->name = name;
    typed = user;
    this->justSeCaPlayer =nullptr;
}

void UserPlayer::prepareCallLord()
{

}

void UserPlayer::prepareSendCards()
{

}
