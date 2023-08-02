#include "qgameplayer.h"

QGamePlayer::QGamePlayer(const QString &name, const QString &avator)
    :m_name(name)
    ,m_avator(avator)
    ,m_gameCard(nullptr)
{

}

void QGamePlayer::setCard(QGameCard *gameCard)
{
    m_gameCard = gameCard;
}
