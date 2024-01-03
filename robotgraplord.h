#ifndef ROBOTGRAPLORD_H
#define ROBOTGRAPLORD_H

#include <QThread>
#include"player.h"
/*
机器人叫地主线程类
*/

class RobotGrapLord : public QThread
{
    Q_OBJECT
public:
    explicit RobotGrapLord(Player* player,QObject *parent = nullptr);

    void run();
signals:

private:
    Player *m_player;
};

#endif // ROBOTGRAPLORD_H
