#ifndef CARDS_H
#define CARDS_H

#include "card.h"
#include <QSet>
#include <QRandomGenerator>

//玩家的卡牌存放地
//可以存储每个玩家手里的扑克牌
//也可以存储整副扑克牌
using QCardList = QList<Card>;
class Cards
{
public:
    //排序方式
    enum  sortType
    {
        up,
        dec
    };
    Cards();
    Cards(Card&card);
    void add(Card&card);
    void add(Card&&card);
    void add(const Cards&cards);
    void add(const QVector<Cards>& cards);

    void remove(Card&card);
    void remove(const Cards &cards);
    int count() const;
    void clear();
    bool isContains(Card&card);
    bool isContains(Cards &cards);
    bool isEmpty() const;
    Cards&operator<<(Card& card);
    Cards&operator<<(Cards cards);
    void remove(const QVector<Cards>& cards);
    //最小点数
     Card::CardPoint getMinPoint();
    //最大点数
     Card::CardPoint getMaxPoint();
     //计算指定点数牌的数量
     int getCardCount(Card::CardPoint point);

     //随机取出一张扑克牌
     Card randSendCard();
     //玩家的卡牌转换成有序的进行展示
    QCardList  toSortList(sortType type = Cards::dec);
     //hash表
    QSet<Card>cards;
};
#endif // CARDS_H
