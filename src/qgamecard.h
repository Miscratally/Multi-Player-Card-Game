#ifndef QGAMECARD_H
#define QGAMECARD_H

#include <QString>

// Enumeration for different card suits.
enum class EnumSuit
{
    Spade = 0,
    Heart = 1,
    Diamond = 2,
    Club = 3,
};

class QGameCard
{
public:
    // Default constructor.
    QGameCard();

    // Constructor to initialize the card with the given number and suit.
    QGameCard(int number, EnumSuit suit);

    // Returns a string representation of the card.
    QString cardString() const;

    // Compares this card with another card.
    // Returns:
    //   1 if this card is greater than the other card.
    //   0 if this card is equal to the other card.
    //  -1 if this card is smaller than the other card.
    int compare(QGameCard* other) const;

    // Returns the number of the card.
    int number() const;

    // Returns the suit of the card.
    EnumSuit suit() const;

private:
    int m_number; // Card number. Aces are valued at 1, Jacks at 11, Queens at 12, and Kings at 13.
    EnumSuit m_suit; // Suit hierarchy is Spades > Hearts > Diamonds > Clubs.
};

#endif // QGAMECARD_H
