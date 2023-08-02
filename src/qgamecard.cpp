#include "qgamecard.h"

QGameCard::QGameCard()
{

}

QGameCard::QGameCard(int number, EnumSuit suit)
    : m_number(number) // Initialize m_number with the provided number
    , m_suit(suit) // Initialize m_suit with the provided suit
{

}

// Returns a QString representation of the card
QString QGameCard::cardString() const
{
    QString str;
    if (m_number >= 0 && m_number <= 9)
        str = QString::number(m_number + 1); // For numbers 1 to 10, use their respective numeric values
    else if (m_number == 10)
        str = "J"; // For number 11 (J), use "J" as the string representation
    else if (m_number == 11)
        str = "Q"; // For number 12 (Q), use "Q" as the string representation
    else if (m_number == 12)
        str = "K"; // For number 13 (K), use "K" as the string representation

    static const QString suitSymbols[] = {u8"♠", u8"♥", u8"♦", u8"♣"}; // Unicode symbols for each suit
    str += suitSymbols[static_cast<int>(m_suit)];

    return str;
}

// Compare the current card with another card (other)
int QGameCard::compare(QGameCard* other) const
{
    if (m_number > other->number())
        return 1;

    if (m_number < other->number())
        return -1; // Return -1 if the current card's number is less than the other card's number

    // If the card numbers are equal, compare the suits using EnumSuit values
    return static_cast<int>(m_suit) - static_cast<int>(other->suit());
}

int QGameCard::number() const
{
    return m_number;
}

EnumSuit QGameCard::suit() const
{
    return m_suit;
}
