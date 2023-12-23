#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <QMainWindow>
#include "card.h"
#include "gamecontrol.h"
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
    // 裁剪图片
    void cropImage(QPixmap& pix, int x, int y, Card& c);
    // 初始化玩家在窗口中的上下文环境
    void initPlayerContext();
    // 初始化游戏场景
    void initGameScene();
    // 处理游戏的状态
    void gameStatusPrecess(GameControl::GameStatus status);

    void paintEvent(QPaintEvent *ev);
private:
    QPixmap m_bkImage;
    Ui::GamePanel *ui;
};
#endif // GAMEPANEL_H
