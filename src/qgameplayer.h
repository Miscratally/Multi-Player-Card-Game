#ifndef QGAMEPLAYER_H
#define QGAMEPLAYER_H

#include <QString>
#include "qgamecard.h"

class QGamePlayer
{
public:
    // Constructor for QGamePlayer class
    QGamePlayer(const QString& name, const QString& avator);

    // Returns The name of the player as a QString
    QString name() const;

    // Returns The avatar of the player as a QString
    QString avator() const;

    // Returns A pointer to the QGameCard object representing the player's game card
    QGameCard *gameCard() const;

    // Set the card of the player
    void setGameCard(QGameCard* newGameCard);

private:
    QString m_name;         // Player name
    QString m_avator;       // Player avator
    QGameCard* m_gameCard;  // Pointer to the player's game card
};

#endif // QGAMEPLAYER_H
