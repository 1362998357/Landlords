#ifndef CARD_H
#define CARD_H
#include <QHash>
//单张卡牌类
class Card
{
public:
    enum CardSuit{
        SuitBegin,
        Diamond, //方块
        Clud,  //梅花
        Heart,//红桃
        Spade,//黑桃
        SuitEnd
    };

    //点数
    enum CardPoint{
        PointBegin,
        Card_3,
        Card_4,
        Card_5,
        Card_6,
        Card_7,
        Card_8,
        Card_9,
        Card_10,
        Card_J,
        Card_Q,
        Card_K,
        Card_A,
        Card_2,
        Card_SJ,
        Card_BJ,
        PointEnd
    };

    Card();
    Card(CardPoint cp,CardSuit csuit);
    //花
    void setPoint(CardPoint carP);
    void setSuit(CardSuit carS);
    //比较相等策略
    bool operator == (const Card&card)const;
    CardPoint Point;
    CardSuit Suit;
};
//排序策略函数
bool lessFunc(Card &c1,Card &c2);
bool greatFunc(Card &c1,Card &c2);
//Qset 下标采用的hash算法函数
 size_t qHash(const Card&card);
#endif // CARD_H
