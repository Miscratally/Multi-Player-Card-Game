#include "qgameplayer.h"

// Constructor for QGamePlayer class
QGamePlayer::QGamePlayer(const QString &name, const QString &avator)
    :m_name(name)
    ,m_avator(avator)
    ,m_gameCard(nullptr) // Initialize m_gameCard to nullptr in the constructor
{

}

// Setter method to set the game card for the player
void QGamePlayer::setGameCard(QGameCard *newGameCard)
{
    m_gameCard = newGameCard;
}

// Getter method to retrieve the game card pointer
QGameCard *QGamePlayer::gameCard() const
{
    return m_gameCard;
}

// Getter method to retrieve the player's avator.
QString QGamePlayer::avator() const
{
    return m_avator;
}

// Getter method to retrieve the player's name
QString QGamePlayer::name() const
{
    return m_name;
}
