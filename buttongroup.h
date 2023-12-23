#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>
#include "startbutton.h"
/*
开始游戏后下方显示的按钮组
*/
namespace Ui {
class ButtonGroup;
}

class ButtonGroup : public QWidget
{
    Q_OBJECT
public:
    //对应ui 中每个page的索引
    enum Panel{Start, PlayCard, PassOrPlay, CallLord, Empty};
    explicit ButtonGroup(QWidget *parent = nullptr);
    void initButtons();
    ~ButtonGroup();
    // 处理page页的切换
    void selectPanel(Panel type, int bet = 0);

signals:
    // 开始游戏
    void startGame();
    // 出牌
    void playHandSendCard();
    // 不出牌
    void passSendCard();
    // 抢地主下注
    void betPoint(int bet);
private:
    Ui::ButtonGroup *ui;
};

#endif // BUTTONGROUP_H
