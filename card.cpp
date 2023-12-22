#include "card.h"
bool lessFunc(Card &c1,Card &c2)
{
    if(c1.Point == c2.Point)
    {
        return c1.Suit<c2.Suit;
    }
    return c1.Point<c2.Point;
}
bool greatFunc(Card &c1,Card &c2)
{
    if(c1.Point == c2.Point)
    {
        return c1.Suit>c2.Suit;
    }
    return c1.Point>c2.Point;
}

Card::Card()
{

}

Card::Card(CardPoint cp, CardSuit csuit)
{
    setPoint(cp);
    setSuit(csuit);
}

void Card::setPoint(CardPoint CardP)
{
    Point = CardP;

}

void Card::setSuit(CardSuit CardS)
{
    Suit = CardS;
}

bool Card::operator ==(const Card &card) const
{
    if(card.Point == Point && card.Suit == Suit)
        return true;
    return false;
}

size_t qHash(const Card &card)
{
    return card.Point*100+card.Suit;
}
