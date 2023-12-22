#ifndef ROBOT_H
#define ROBOT_H

#include "player.h"
#include <QObject>

class Robot : public Player
{
    Q_OBJECT
public:
    explicit Robot(QString name,QObject *parent = nullptr);
    using Player::Player;

    //准备叫地主
    virtual void prepareCallLord() override;
    //准备出牌
    virtual void prepareSendCards() override;
};

#endif // ROBOT_H
