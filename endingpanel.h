#ifndef ENDINGPANEL_H
#define ENDINGPANEL_H
/*
游戏结束结算画面
*/
#include "scorepanel.h"

#include <QLabel>
#include <QPushButton>
#include <QWidget>

class EndingPanel : public QWidget
{
    Q_OBJECT
public:
    explicit EndingPanel(bool isLord, bool isWin, QWidget *parent = nullptr);
    //设置面版上的结算分数
    void setPlayerScore(int left, int right, int me);

signals:
    //继续游戏信号
    void continueGame();

protected:
    void paintEvent(QPaintEvent* ev);

private:
    QPixmap m_bk;
    QLabel* m_title;
    scorePanel *m_score;
    //继续游戏按钮
    QPushButton* m_continue;

};

#endif // ENDINGPANEL_H
