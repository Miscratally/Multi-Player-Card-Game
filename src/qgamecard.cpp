#include "qgamecard.h"

QGameCard::QGameCard()
{

}

QGameCard::QGameCard(int number, EnumSuit suit)
    :m_number(number)
    ,m_suit(suit)
{

}

QString QGameCard::cardString() const
{
    QString str;
    if(m_number>=0&&m_number<=9)
        str = QString::number(m_number+1);
    else if(m_number == 10)
        str = "J";
    else if(m_number == 11)
        str = "Q";
    else if(m_number == 12)
        str = "K";

    switch(m_suit)
    {
    case kSpade:
        str += u8"♠";
        break;
    case kHeart:
        str += u8"♥";
        break;
    case kDiamond:
        str += u8"♦";
        break;
    case kClub:
        str += u8"♣";
        break;
    default:
        break;
    }

    return str;
}
