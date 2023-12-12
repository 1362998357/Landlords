#ifndef CARDPANEL_H
#define CARDPANEL_H

#include <QWidget>
#include <QPainter>
#include "card.h"
#include "player.h"
class CardPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CardPanel(QWidget *parent = nullptr);

    //获取图片
    void setImage(QPixmap frontImage,QPixmap backImage);
    QPixmap getImage();
    void setShowFront(bool flage);
    //返回扑克牌是否是正面
    bool isShowFront();

    //设置是否被选中
    void setSeleted(bool flage);
    bool isSelected();

    //扑克牌的花色以及点数
    void setCard(Card car);
    Card getCard();

    //扑克牌的所有者
    void setOwner(Player* player);
    Player*getPlayer();


    //事件处理函数
    void paintEvent(QPaintEvent*event);
    void mousPressEvernt(QMouseEvent *event);
private:
    QPixmap front;
    QPixmap back;
    bool isFront;
    bool selected;
    Card card;
    Player*owner;
signals:

};

#endif // CARDPANEL_H
