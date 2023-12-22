#include "robot.h"
#include <QDebug>
Robot::Robot(QString name,QObject *parent)
    : Player{parent}
{
    this->name = name;
    typed = robot;
}

void Robot::prepareCallLord()
{

}

void Robot::prepareSendCards()
{

}
