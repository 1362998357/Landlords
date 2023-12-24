#include "cardpanel.h"

CardPanel::CardPanel(QWidget *parent)
    : QWidget{parent}
{
    isFront = false;

}

void CardPanel::setImage(QPixmap frontImage, QPixmap backImage)
{
        front = frontImage;
        back = backImage;
        //设置窗口大小和扑可拍大小一致
        setFixedSize(front.size());
        update();
}

QPixmap CardPanel::getImage()
{
    return front;
}

void CardPanel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(isFront)
    {
        painter.drawPixmap(rect(),front);
    }
    else
    {
        painter.drawPixmap(rect(),back);
    }
}

void CardPanel::mousPressEvernt(QMouseEvent *event)
{

}

void CardPanel::setShowFront(bool flage)
{
    isFront = flage;
}

bool CardPanel::isShowFront()
{
    return isFront;
}

void CardPanel::setSeleted(bool flage)
{
    selected = flage;
}

bool CardPanel::isSelected()
{
    return selected;
}

void CardPanel::setCard(Card car)
{
    this->card= car;
}

Card CardPanel::getCard()
{
    return card;
}

void CardPanel::setOwner(Player *player)
{
     owner = player;
}

Player *CardPanel::getPlayer()
{
    return owner;
}
