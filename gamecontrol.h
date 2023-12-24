#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QObject>
#include <QSize>
#include <QPixmap>
#include "robot.h"
#include"userplayer.h"
#include"cardpanel.h"
/*
游戏控制类玩家的各种操作的实现
*/
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
    Card takeOneCard();
    //获取最后的3张底牌
    Cards getSurplusThreeCards();
    //初始化卡牌
    void initCards();
    //重新清空数据/开始洗牌
    void resetCardDate();
    //开始叫地主
    void statrCallLord();
    void becomeLord(Player*player);
    void clearScore();
    // 处理玩家选牌
    void onCardSelected(Qt::MouseButton button);
    // 处理用户玩家出牌
    void onUserPlayHand();
    void clearPlayerScore();
    // 裁剪图片
    void cropImage(QPixmap& pix, CardPanel*panel,int x, int y, Card& c);

private:
    Robot*leftRobot = nullptr;
    Robot*rightRobot = nullptr;
    UserPlayer*userPlayer= nullptr;
    //当前玩家
    Player*currentPlayer = nullptr;
    //出牌玩家
    Player*sendHandPlayer = nullptr;
    Cards sendHandPlayerCards;
    Cards allCards;
public:
    //卡牌大小
    QSize m_cardSize;
    //卡牌的图片
    QPixmap m_cardBackImg;
signals:

};

#endif // GAMECONTROL_H
