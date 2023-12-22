#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QObject>
#include "robot.h"
#include"userplayer.h"
class GameControl : public QObject
{
    Q_OBJECT
public:
    explicit GameControl(QObject *parent = nullptr);
    //游戏状态
    enum GameStatus
    {
        //发牌状态
        sendCard,
        //叫地主状态
        callLords,
        //出牌状态
        handSendCard
    };
    enum PlayStatus
    {
        //获胜
        win,
        //考虑叫地主状态
        thinkCallLords,
        //考虑出牌状态
        thinkHandSendCard
    };
    //初始化游戏玩家
    void playerInint();
    //得到玩家的实例对象
    Robot*getLeftRobt();
    Robot*getRightRobt();
    UserPlayer*getUserPlayer();
   //设置当前玩家
    void setCurrentPlayer(Player*player);
    //获取当前玩家
    Player*getCurrentPlayer();
    //得到出牌玩家打出的牌
    Cards getSendHandCards();
    //随机发一张牌给玩家
    void takeOneCard();
    //获取最后的3张底牌
    Cards getSurplusThreeCards();
    //初始化卡牌
    void initCards();
    //重新洗牌
    void resetCardDate();
    //开始叫地主
    void statrCallLord();
    void becomeLord(Player*player);
    void clearScore();

private:
    Robot*leftRobot;
    Robot*rightRobot;
    UserPlayer*userPlayer;
    //当前玩家
    Player*currentPlayer;
    //出牌玩家
    Player*sendHandPlayer;
    Cards sendHandPlayerCards;
    Cards allCards;
signals:

};

#endif // GAMECONTROL_H
