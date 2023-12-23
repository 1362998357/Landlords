#include "startbutton.h"
#include <QPainter>
startButton::startButton(QWidget *parent)
    :QPushButton {parent}
{

}

void startButton::setImage(QString norml, QString entr, QString press)
{
    normlPicPath = norml;
    pressPicPath = press;
    entrPicPath = entr;
    statrButPic.load(norml);
    update();

}

void startButton::enterEvent(QEnterEvent *ev)
{
    statrButPic.load(entrPicPath);
    update();
}

void startButton::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        statrButPic.load(pressPicPath);
    }
    update();
    //调用click信号
    QPushButton::mousePressEvent(ev);
}

void startButton::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        statrButPic.load(entrPicPath);
    }
    update();
    //调用click信号
    QPushButton::mouseReleaseEvent(ev);
}

void startButton::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    painter.drawPixmap(rect(),statrButPic);
}

void startButton::leaveEvent(QEvent *ev)
{
    statrButPic.load(normlPicPath);
    update();
}
