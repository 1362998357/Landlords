#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QTimer>
#include <QWidget>
/*
闹钟定时器按钮定时显示
*/
class CountDown : public QWidget
{
    Q_OBJECT
public:
    explicit CountDown(QWidget *parent = nullptr);
//显示
    void showCountDown();
//隐藏
    void stopCountDown();

signals:
    //没有太多的时间了播放提示音乐
    void notMuchTime();
    //时间没了，给游戏面板类发送信号
    void timeout();

protected:
    void paintEvent(QPaintEvent* ev);

private:
    QPixmap m_clock;//闹钟图片
    QPixmap m_number;//闹钟要显示的数字
    QTimer* m_timer;//定时器
    int m_count;//定时事件

};

#endif // COUNTDOWN_H
