#ifndef QGAMECARD_H
#define QGAMECARD_H
#include <QString>
enum EnumSuit
{
    kSpade =0,
    kHeart =1,
    kDiamond =2,
    kClub =3,
};

class QGameCard
{
public:
    QGameCard();
    QGameCard(int number,EnumSuit suit);
    QString cardString() const;

    // 1 = big  0 = euqal -1 = small
    int compare(QGameCard* other) const{
        if(m_number > other->m_number)
            return 1;

        if(m_number ==other->m_number)
        {
            if(m_suit < other->m_suit)
                return 1;

            if(m_suit==other->m_suit)
                return 0;
        }

        return -1;
    }
public:
    int m_number;        //card number Aces are valued at 1, Jacks at 11, Queens at 12, and Kings at 13.
    EnumSuit m_suit;          //Suit hierarchy is Spades > Hearts > Diamonds > Clubs.
};

#endif // QGAMECARD_H
