#ifndef QGAMEPLAYER_H
#define QGAMEPLAYER_H
#include <QString>
#include "qgamecard.h"
class QGamePlayer
{
public:
    QGamePlayer(const QString& name,const QString& avator);
    void setCard(QGameCard* gameCard);
public:
    QString m_name;         //player name
    QString m_avator;       //player avator

    QGameCard* m_gameCard;
};

#endif // QGAMEPLAYER_H
