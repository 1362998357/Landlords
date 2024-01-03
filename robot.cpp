#include "robot.h"
#include "robotgraplord.h"
#include "robotplayhand.h"
#include"strategy.h"
#include <QDebug>
Robot::Robot(QString name,QObject *parent)
    : Player{parent}
{
    this->name = name;
    this->justSeCaPlayer =nullptr;
    typed = robot;
}

void Robot::prepareCallLord()
{
    //创建机器人叫地主线程
    RobotGrapLord* subThread = new RobotGrapLord(this);
    //线程回收
    connect(subThread, &RobotGrapLord::finished, this, [=](){
        qDebug() << "RobotGrapLord 子线程对象析构..." << ", Robot name: " << this->getName();
                                            subThread->deleteLater();
    });
    subThread->start();
}

void Robot::prepareSendCards()
{
    RobotPlayHand* subThread = new RobotPlayHand(this);
    connect(subThread, &RobotGrapLord::finished, this, [=](){
        qDebug() << "RobotPlayHand 子线程对象析构..." << ", Robot name: " << this->getName();
                                            subThread->deleteLater();
    });

    subThread->start();
}

void Robot::thinkCallLords()
{
    qDebug()<<"thinkcallLrords "<<"Robot name "<<this->getName();
    /*
     * 基于手中的牌计算权重
     * 大小王: 6
     * 顺子/炸弹: 5
     * 三张点数相同的牌: 4
     * 2的权重: 3
     * 对儿牌: 1
    */
    int weight = 0;
    Strategy st(this, myCards);
    //得到手里的大小王的权重
    weight += st.getRangeCards(Card::Card_SJ, Card::Card_BJ).count() * 6;
    //得到手里顺子的权重
    QVector<Cards> optSeq = st.pickOptimalSeqSingles();
    weight += optSeq.size() * 5;
    //得到手里炸弹的权重
    QVector<Cards> bombs = st.findCardsByCount(4);
    weight += bombs.size() * 5;

    weight += myCards.getCardCount(Card::Card_2) * 3;

    Cards tmp = myCards;
    //防止上一次的炸弹和3张里包括这个对子 连对不需要处理因为函数逻辑有处理
    tmp.remove(optSeq);
    tmp.remove(bombs);
    Cards card2 = st.getRangeCards(Card::Card_2, Card::Card_2);
    tmp.remove(card2);
    QVector<Cards> triples = Strategy(this, tmp).findCardsByCount(3);
    weight += triples.size() * 4;

    tmp.remove(triples);
    QVector<Cards> pairs = Strategy(this, tmp).findCardsByCount(2);
    weight += pairs.size() * 1;

    if(weight >= 22)
    {
        grabLordBet(3); //叫地主下注
    }
    else if(weight < 22 && weight >=18)
    {
        grabLordBet(2);
    }
    else if(weight < 18 && weight >= 10)
    {
        grabLordBet(1);
    }
    else
    {
        grabLordBet(0);
    }

}
void Robot::thinkPlayHand()
{

    qDebug()<<"Robot thinkPlayHand robot think playeHand 91";
    Strategy st(this, myCards);
    //机器人考虑出什么样的牌 采取策略
    qDebug()<<"Robot thinkPlayHand robot think playeHand 97";
    Cards cs = st.makeStrategy();
    qDebug()<<"Robot thinkPlayHand robot think playeHand 98";
    //出牌
    playerOutCard(cs);
}
