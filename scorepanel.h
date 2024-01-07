#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>
#include <QLabel>
/*
 * 分数面版
*/
namespace Ui {
class scorePanel;
}

class scorePanel : public QWidget
{
    Q_OBJECT

public:
    enum FontColor{Black, White, Red, Blue, Green};
    explicit scorePanel(QWidget *parent = nullptr);
    // 设置玩家的得分
    void setScores(int left,int right,int user);
    // 设置字体大小
    void setMyFontSize(int point);
    // 设置字体的颜色
    void setMyFontColor(FontColor color);

    ~scorePanel();

private:
    Ui::scorePanel *ui;
    //存放分数面板中的各个label的指针
    QVector<QLabel*> m_list;
};

#endif // SCOREPANEL_H
