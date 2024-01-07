#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <QMainWindow>
#include "animationwindow.h"
#include <QLabel>
#include "card.h"
#include <QTimer>
#include "cardpanel.h"
#include "gamecontrol.h"
#include "bgmcontrol.h"
#include "countdown.h"
/*
 * 游戏面板类将游戏面板中的各种组件显示出来
*/
QT_BEGIN_NAMESPACE
namespace Ui { class GamePanel; }
QT_END_NAMESPACE

class GamePanel : public QMainWindow
{
    Q_OBJECT

public:
    GamePanel(QWidget *parent = nullptr);
    ~GamePanel();    enum AnimationType{ShunZi, LianDui, Plane, JokerBomb, Bomb, Bet};
    // 初始化游戏控制类信息
    void gameControlInit();
    // 更新分数面板的分数
    void updatePlayerScore();
    // 切割并存储图片
    void initCardMap();
    // 初始化玩家在窗口中的上下文环境
    void initPlayerContext();
    // 初始化游戏场景
    void initGameScene();
    // 处理游戏的状态 如出牌 洗牌 发牌
    void gameStatusPrecess(GameControl::GameStatus status);
    // 初始化游戏按钮组
    void initButtonsGroup();
    void onCardSelected(Qt::MouseButton button);   // 处理玩家选牌槽函数
    // 处理用户玩家出牌
    void onUserPass();
    // 处理用户玩家出牌
    void onUserPlayHand();
    //开始发牌
    void startDispatchCard();
    //定时器处理函数 处理发牌
    void onDispatchCard();
    //发牌时移动一步处理函数
    void cardMoveStep(Player *, int);
    //显示扑克牌到扑克牌显示区域
    void disposeCard(Player *player, const Cards &cards);
    // 更新扑克牌在窗口中的显示
    void updatePlayerCards(Player *player);
    // 处理玩家状态的变化
    void onPlayerStatusChanged(Player* player, GameControl::PlayStatus status);
    //更新下注时要显示的提示信息
    void onGrabLordBet(Player*,int, bool);
    //展示动画
    void showAnimation(AnimationType type, int bet= 0);
    // 处理玩家的出牌
    void onDisposePlayHand(Player* player, const Cards& cards);
    // 隐藏玩家打出的牌
    void hidePlayerDropCards(Player* player);
    //加载玩家头像 转换成pixmap的形式
    QPixmap loadRoleImage(Player::Sex sex, Player::Derict direct, Player::Role role);
    // 显示玩家的最终得分
    void showEndingScorePanel();
    // 初始化闹钟倒计时
    void initCountDown();
protected:
    void paintEvent(QPaintEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
private:
    enum CardAlign{Horizontal, Vertical};
    //玩家上下文信息结构体
    struct PlayerContext
    {
        // 1. 玩家扑克牌显示的区域
        QRect cardRect;
        // 2. 出牌的区域
        QRect playHandRect;
        // 3. 扑克牌的对齐方式(水平 or 垂直)
        CardAlign align;
        // 4. 扑克牌显示正面还是背面
        bool isFrontSide;
        // 5. 游戏过程中的提示信息, 比如: 不出
        QLabel* info;
        // 6. 玩家的头像
        QLabel* roleImg;
        // 7. 玩家刚打出的牌
        Cards lastCards;
    };
    //背景图片
    QPixmap m_bkImage;
    //玩家容器
    QList<Player*> PlayerList;
    //玩家游戏中上下文容器
    QMap<Player*, PlayerContext> m_contextMap;
    //每张卡牌面板map
    QMap<Card,CardPanel*> CardPenalMap;
    //游戏开始发牌区的扑克牌
    CardPanel* m_baseCard;
    //发牌过程要移动的扑克牌
    CardPanel* m_moveCard;
    //最后3张底牌
    QVector<CardPanel*> m_last3Card;
    //
    //发牌区位置
    QPoint m_baseCardPos;
    //游戏状态
    GameControl::GameStatus stat;
    //游戏控制类
    GameControl* gc;
    Ui::GamePanel *ui;
    //发牌定时器
    QTimer* m_timer;
    //发牌音乐
    //BGMControl* m_bgm;
    QRect m_cardsRect;
    //用户玩家的卡牌手里的牌方便使用
    QHash<CardPanel*, QRect> m_userCards;
    //显示的动画类
    AnimationWindow *m_animation;
    //保存被选中的卡片
    CardPanel* m_curSelCard;
    QSet<CardPanel*> m_selectCards;
    CountDown* m_countDown;
    BGMControl* m_bgm;
};
#endif // GAMEPANEL_H
