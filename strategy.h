#ifndef STRATEGY_H
#define STRATEGY_H
#include "player.h"
#include "playhand.h"

class Strategy
{
public:
    Strategy(Player *player, const Cards &cards);

    // 1. 制定出牌策略机器人使用
    Cards makeStrategy();
    // 2. 第一个出牌 firstPlay
    Cards firstPlay();
    // 3. 得到比指定牌型大的牌
    Cards getGreaterCards(PlayHand type);
    // 4. 能大过指定的牌时，判断是出牌还是放行, 返回true->出牌, 返回false->放行
    bool whetherToBeat(Cards& cs);

    // 5. 找出指定数量(count)的相同点数的牌(point), 找出count张点数为point的牌
    Cards findSamePointCards(Card::CardPoint point, int count);
    // 6. 找出所有点数中牌的数量为count的牌 ==> 得到一个多张扑克牌数组
    QVector<Cards> findCardsByCount(int count);
    // 7. 根据点数范围找牌
    Cards getRangeCards(Card::CardPoint begin, Card::CardPoint end);
    // 8. 按牌型(对子还是顺子还是连对)）（不是具体到点数）找牌，并且指定要找的牌是否要大过指定的牌型
    //如果beat=true 则从出牌类型点数后面找，也就是找到所有大于这个牌型的点数的相同类型的牌
    //如果beat = false 则就是获得所有和出牌类型相同类型的牌不关心点数
    QVector<Cards> findCardType(PlayHand hand, bool beat);

    // 9. 从指定的Cards对象中挑选出满足条件的顺子
    void pickSeqSingles(QVector<QVector<Cards>> &allSeqRecord, const QVector<Cards> &seqSingle, const Cards& cards);
    // 10. 最优的顺子的集合的筛选函数
    QVector<Cards> pickOptimalSeqSingles();

private:
    //连对信息函数
    using function = Cards (Strategy::*)(Card::CardPoint point);
    struct CardInfo
    {
        Card::CardPoint begin;
        Card::CardPoint end;
        int extra;  // 顺子或者连对的数量
        bool beat;  //是否根据点数去找值
        int number; // 指定点数的牌的数量
        int base;   // 最基础的顺子或者连对的数量 顺子5 连对3
        function getSeq;//创建基础连对的函数getBaseSeqPair getBaseSeqSingle
    };
    //（隐含获得比点数之前获得的牌型大）
    //从point开始获得所有的 number张相同点数的牌
    QVector<Cards> getCards(Card::CardPoint point, int number);
    //从点数point开始查找3带一对的牌
    QVector<Cards> getTripleSingleOrPair(Card::CardPoint begin, PlayHand::HandType type);
    //从点数point开始查找飞机
    QVector<Cards> getPlane(Card::CardPoint begin);
    //从点数point开始查找飞机带2单
    QVector<Cards> getPlane2SingleOr2Pair(Card::CardPoint begin, PlayHand::HandType type);
    //获得连对或者顺子
    QVector<Cards> getSepPairOrSeqSingle(CardInfo &info);
    Cards getBaseSeqPair(Card::CardPoint point);
    Cards getBaseSeqSingle(Card::CardPoint point);
    //获得炸弹
    QVector<Cards> getBomb(Card::CardPoint begin);

private:
    Player* m_player;
    Cards m_cards;//玩家手里的牌

};

#endif // STRATEGY_H
