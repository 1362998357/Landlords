#ifndef STARTBUTTON_H
#define STARTBUTTON_H
/*
 * 开始按钮类
*/
#include <QPushButton>
#include<QMouseEvent>
#include<QEnterEvent>
#include<QPaintEvent>

class startButton : public QPushButton
{
    Q_OBJECT
public:
    explicit startButton(QWidget *parent = nullptr);
    void setImage(QString norml,QString entr,QString press);
signals:
protected:
    void enterEvent(QEnterEvent *ev);
    void mousePressEvent(QMouseEvent*ev);
    void mouseReleaseEvent(QMouseEvent*ev);
    void paintEvent(QPaintEvent*ev);
    void leaveEvent(QEvent*ev);
    //开始按钮加载的图片和路径
    QPixmap statrButPic;
    QString normlPicPath;
    QString pressPicPath;
    QString entrPicPath;
};

#endif // STARTBUTTON_H
