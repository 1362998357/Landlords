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

//记录玩家下注信息类
struct BetRecord
{
    BetRecord()
    {
        reset();
    }
    //清空玩家下注信息
    void reset()
    {
        player = nullptr;
        bet = 0;
        times = 0;
    }
    Player* player;//下注的玩家
    int bet;//下注的分数
    int times;  // 第几次叫地主
};

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
    //成为地主
    void becomeLord(Player*player,int &bet);
    void clearScore();
    // 处理叫地主
    void onGrabBet(Player* player, int bet);
    // 处理玩家选牌
    void onCardSelected(Qt::MouseButton button);
    // 处理用户玩家出牌
    void onUserPlayHand();
    // 处理出牌
    void onPlayHand(Player *player, const Cards &card);
    //清空玩家分数
    void clearPlayerScore();
    // 裁剪图片
    void cropImage(QPixmap& pix, CardPanel*panel,int x, int y, Card& c);
    //获得最大的下注分数
    int getPlayerMaxBet();


private:
    Robot*leftRobot = nullptr;
    Robot*rightRobot = nullptr;
    UserPlayer*userPlayer= nullptr;
    //当前玩家
    Player*currentPlayer = nullptr;
    //出牌玩家
    Player*sendHandPlayer = nullptr;
    //发出的牌
    Cards sendHandPlayerCards;
    //所有的牌
    Cards allCards;
    //存储玩家下注信息类
    BetRecord m_betRecord;
    int m_curBet = 0;
public:
    //卡牌大小
    QSize m_cardSize;
    //卡牌的图片
    QPixmap m_cardBackImg;
signals:
    void playerStatusChanged(Player* player, PlayStatus status);
    // 通知玩家抢地主了
    void notifyGrabLordBet(Player* player, int bet, bool flag);
    // 游戏状态变化
    void gameStatusChanged(GameStatus status);
    // 通知玩家出牌了
    void notifyPlayHand(Player* player, const Cards& card);
    // 给玩家传递出牌数据
    void pendingInfo(Player* player, const Cards& card);
};

#endif // GAMECONTROL_H
