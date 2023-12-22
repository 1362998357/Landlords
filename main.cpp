#include "gamepanel.h"

#include <QApplication>
#include "robot.h"
#include "userplayer.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GamePanel w;
    //斗地主开始
    Robot *bo = new Robot("sad");
    UserPlayer *po = new UserPlayer("sassd");
    if(bo->getTyped() == Player::robot)
    {
        qDebug()<<bo->getName()<<"\n";
        qDebug()<<"robot \n";
    }
    if(po->getTyped()&Player::user)
    {
        qDebug()<<"user\n";
    }
    w.show();
    return a.exec();
}
