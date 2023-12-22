#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QString>
#include "cards.h"
class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);
    explicit Player(QString name,QObject *parent = nullptr);
    enum Derict{left,right};
    enum Sex{man,women};
    enum Typed{robot,user,unkown};
    enum Role{lord,farmer};
    //姓名
    void setName(QString name);
    QString getName();
    //头像显示方向
    void setDirection(Derict derict);
    Derict getDirection();
    //角色
    void setRole(Role role);
    Role getRole();
    //性别
    void setSex(Sex sex);
    Sex getSex();
    //玩家类型
    void setTyped(Typed typed);
    Typed getTyped();
    //玩家分数
    void setScore(int score);
    int getScore();
    //判断是否已经赢了
    void setIswin(bool isWin);
    bool getIsWin();

    //存储手里的牌
    void storeCard(Card &card);
    void storeCards(Cards &cards);

    //得到手里的所有的牌
    Cards getCards();
    //清空所有的牌
    void clearMyCards();
    //出牌
    void playerOutCard(Cards &cards);
    //出牌顺序前后玩家
    void setPrePlayer(Player* player);
    void setNextPlayer(Player *player);

    Player* getPrePlayer();
    Player* getNextPlayer();
    //抢地主 下注
    void grabLordBet(int point);

    //记录用户刚打出的牌，其他人要根据这个牌进行选择出牌
    void storeSendCardsInfor(Cards cards,Player *player);
    Cards getJustSendCards();
    Player* getJustSendPlayer();
    //准备叫地主
    virtual void prepareCallLord();
    //准备出牌
    virtual void prepareSendCards();

protected:
    QString name;
    Derict derict;
    Sex sex;
    Typed typed;
    Role role;

    int score;
    bool isWin;

    //手里的牌
    Cards myCards;
    Player *prePlayer;
    Player *nextPlayer;

    //记录用户打出的牌
    Cards justSendCards;
    Player *justSeCaPlayer;

signals:

};

#endif // PLAYER_H
