#include "cards.h"

Cards::Cards()
{

}

Cards::Cards(Card &card)
{
    this->add(card);
}

void Cards::add(Card &card)
{
    this->cards.insert(card);
}

void Cards::add(Card &&card)
{
    this->cards.insert(card);
}

void Cards::add(const Cards &cards)
{
    this->cards.unite(cards.cards);
}

void Cards::add(const QVector<Cards> &cards)
{
    for(int i=0; i<cards.count(); ++i)
    {
        add(cards.at(i));
    }
}

void Cards::remove(Card &card)
{
    this->cards.remove(card);
}

void Cards::remove(const  Cards &cards)
{
    this->cards.subtract(cards.cards);
}

int Cards::count() const
{
   return cards.size();
}

void Cards::clear()
{
    cards.clear();
}

bool Cards::isContains(Card &card)
{
    return cards.contains(card);
}

bool Cards::isContains(Cards &cards)
{
    return (this->cards).contains(cards.cards);
}
bool Cards::isEmpty() const
{
    return cards.isEmpty();
}

Cards &Cards::operator<<(Cards card)
{
    add(card);
    return *this;
}

void Cards::remove(const QVector<Cards> &cards)
{
    for(int i=0; i<cards.size(); ++i)
    {
        remove(cards[i]);
    }
}

int Cards::getCardCount(Card::CardPoint cPoint)
{
    int size =0;
    for(auto &point:cards)
    {
        if(point.Point == cPoint)
            size++;
    }
    return size;
}

Card Cards::randSendCard()
{
    //随机取出一张牌发出去
    int v =  QRandomGenerator::global()->bounded(count());
    int j =0;
    auto itr = cards.begin();
    for(;j<v;itr++,j++)
    {

    }
    Card card = *itr;
    cards.erase(itr);//把这张牌从控制类中的牌池中删除
    return card;
}

QList<Card> Cards::toSortList(sortType type)
{
    QList<Card> cardList;
    for(auto &itr:cards)
    {
        cardList<<itr;
    }

    if(type == Cards::up)
    {
        std::sort(cardList.begin(),cardList.end(),lessFunc);
    }
    else if(type==Cards::dec)
    {
        std::sort(cardList.begin(),cardList.end(),greatFunc);
    }
    return cardList;
}
Card::CardPoint Cards::getMinPoint()
{
   Card::CardPoint min = Card::PointBegin;
   if(isEmpty())
   return min;
   for(auto &point:cards)
   {
       if(point.Point!=Card::PointBegin)
       min=std::min(min,point.Point);
   }
   return min;
} 
Card::CardPoint Cards::getMaxPoint()
{
   Card::CardPoint max = Card::PointBegin;
   if(isEmpty())
   return max;
   for(auto &point:cards)
   {
       if(point.Point!=Card::PointEnd)
       max=std::max(max,point.Point);
   }
   return max;
} 
Cards &Cards::operator<<(Card &card)
{
    add(card);
    return *this;
}
