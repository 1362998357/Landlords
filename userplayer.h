#ifndef USERPLAYER_H
#define USERPLAYER_H

#include "player.h"
#include <QObject>

class UserPlayer : public Player
{
    Q_OBJECT
public:
    using Player::Player;
    explicit UserPlayer(QString name,QObject *parent = nullptr);

    //准备叫地主
    virtual void prepareCallLord() override;
    //准备出牌
    virtual void prepareSendCards() override;
signals:
    void startCountDown(); //玩家出牌计时信号
};

#endif // USERPLAYER_H
