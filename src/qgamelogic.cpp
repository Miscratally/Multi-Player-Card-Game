#include <QDebug>
#include <time.h>
#include <vector>
#include <QString>
#include "qgamelogic.h"
#include "mainwindow.h"

const QString g_somePlayerName[] = {
    QStringLiteral("Alice"),
    QStringLiteral("Box"),
    QStringLiteral("Car"),
    QStringLiteral("Paul"),
    QStringLiteral("John"),
    QStringLiteral("Jusin"),
    QStringLiteral("Messi"),
    QStringLiteral("Mask"),
    QStringLiteral("Frank"),
    QStringLiteral("God"),
};

const QString g_somePlayerAvator[] = {
    QStringLiteral("/avator/avator/Anivia_Square_0.png"),
    QStringLiteral("/avator/avator/Annie_square_0.png"),
    QStringLiteral("/avator/avator/Vayne_Square_0.png"),
    QStringLiteral("/avator/avator/Renekton_Square_0.png"),
    QStringLiteral("/avator/avator/MissFortune_square_0.png"),
    QStringLiteral("/avator/avator/Jinx_Square_0.png"),
    QStringLiteral("/avator/avator/Galio_square_0.png"),
    QStringLiteral("/avator/avator/Ashe_square_0.png"),
};

// Constructor
QGameLogic::QGameLogic(MainWindow* mainUi)
    : m_mainUi(mainUi)
{
    // Initialize the player vector
    m_vectorPlayer.resize(g_maxPlayerCnt);
    for (int i = 0; i < g_maxPlayerCnt; i++)
        m_vectorPlayer[i] = nullptr;
}

// Return the randomly selected name.
QString getRandName()
{
    static bool seeded = false; // Use static variable to ensure srand is called only once.
    if (!seeded) {
        srand(time(NULL));
        seeded = true;
    }
    int idx = rand() % g_somePlayerNameCnt; // Generate a random index within the range of g_somePlayerNameCnt.

    return g_somePlayerName[idx];
}

// Return the randomly selected avatar.
QString getRandAvator()
{
    static bool seeded = false;
    if (!seeded) {
        srand(time(NULL));
        seeded = true;
    }
    int idx = rand() % g_somePlayerAvatorCnt; // Generate a random index within the range of g_somePlayerAvatorCnt.

    return g_somePlayerAvator[idx];
}

bool QGameLogic::initGame(int playerCount)
{
    // Check if the player count within the range.
    if (playerCount < g_minPlayerCnt || playerCount > g_maxPlayerCnt )
    {
        return false;
    }

    for(int i =0;i < playerCount; i++)
    {
        QString name = getRandName();

        // Make sure the generated name is unique among players.
        while(findPlayerByName(name)!=nullptr)
        {
            name = getRandName();
        }

        // TODO: Will use same Avator!
        QGamePlayer* player = new QGamePlayer(name,getRandAvator());
        m_vectorPlayer[i] = player;
    }


    // Initialize all 52 cards.
    for (int suit = 0; suit < g_numSuit; suit++)
    {
        for (int number = 0; number < g_numCard; number++)
        {
            QGameCard* gameCard = new QGameCard(number, static_cast<EnumSuit>(suit));
            m_listGameCard.append(gameCard); // Add the card to the list of game cards.
        }
    }

    return true;
}

void QGameLogic::startOneRound()
{
    if(getCardCnt()==0)
        randomGameCards();

    //clear player cards
    for(int i =0;i<g_maxPlayerCnt;i++)
    {
        if(m_vectorPlayer[i])
        {
            m_vectorPlayer[i]->setGameCard(nullptr);
        }
    }

    m_timeCountDown = 14;
}

QGamePlayer *QGameLogic::addPlayer(int idx)
{
    if(idx<0||idx>=g_maxPlayerCnt)
        return nullptr;

    if(m_vectorPlayer[idx] !=nullptr)
        return nullptr;

    QString name = getRandName();
    while(findPlayerByName(name)!=nullptr)
    {
        name = getRandName();
    }

    QGamePlayer* player = new QGamePlayer(name,getRandAvator());
    m_vectorPlayer[idx] =player;

    return player;
}

void QGameLogic::removePlayer(int idx)
{
    delete m_vectorPlayer[idx];
    m_vectorPlayer[idx] = nullptr;
}

void QGameLogic::removePlayer(QGamePlayer *player)
{
    if(m_vectorPlayer.removeOne(player))
        delete player;
}

QGamePlayer *QGameLogic::findPlayerByName(const QString &name)
{
    for(int i = 0;i<m_vectorPlayer.length();i++)
    {
        if(m_vectorPlayer[i])
        {
            if(name == m_vectorPlayer[i]->name())
                return m_vectorPlayer[i];
        }
    }
    return nullptr;
}

QVector<QGamePlayer *> QGameLogic::getAllPlayers()
{
    return m_vectorPlayer;
}

void QGameLogic::randomGameCards()
{
    for(int i =0;i<52;i++)
        m_listCardIdx.append(i);

    for(int i = 52-1; i>0; i--)
    {
        int j = rand() % (i+1);

        int temp = m_listCardIdx[i];
        m_listCardIdx[i] = m_listCardIdx[j];
        m_listCardIdx[j] = temp;
    }

}

QGameCard *QGameLogic::playerDrawCard(int idx)
{
    if(idx<0||idx>=g_maxPlayerCnt)
        return nullptr;

    QGamePlayer*  gamePlayer = m_vectorPlayer[idx];
    if(gamePlayer==nullptr)
        return nullptr;

    if(gamePlayer->gameCard())
        return gamePlayer->gameCard();

    int cardCnt = m_listCardIdx.length();
    if(cardCnt ==0)
        return nullptr;

    QGameCard* gameCard = m_listGameCard.at(m_listCardIdx.at(0));
    m_listCardIdx.removeAt(0);

    gamePlayer->setGameCard(gameCard);

    //if all player draw card
    int playerCnt = 0;
    int playerCardCnt = 0;
    for(int i = 0;i<m_vectorPlayer.length();i++)
    {
        if(m_vectorPlayer[i])
        {
            playerCnt++;
            if(m_vectorPlayer[i]->gameCard())
            {
                playerCardCnt++;
            }
        }
    }
    qDebug() <<"playerCnt="<<playerCnt<<",playerCardCnt="<<playerCardCnt;
    if(playerCnt !=0 && playerCnt==playerCardCnt)
    {
        calcWinner();
    }
    return gameCard;
}

int QGameLogic::getCardCnt()
{
    return m_listCardIdx.length();
}

void QGameLogic::calcWinner()
{
    QGamePlayer* winnerPlayer = nullptr;

    for(int i = 0;i<m_vectorPlayer.length();i++)
    {
        if(m_vectorPlayer[i] && m_vectorPlayer[i]->gameCard())
        {
            if(winnerPlayer==nullptr)
            {
                winnerPlayer = m_vectorPlayer[i];
            }
            else
            {
                QGamePlayer* tmpPlayer = m_vectorPlayer[i];
                if(tmpPlayer && tmpPlayer->gameCard())
                {
                    //calc win
                    if(winnerPlayer->gameCard()->compare(tmpPlayer->gameCard()) < 0)
                    {
                        winnerPlayer = tmpPlayer;
                    }
                }
            }
        }
    }

    m_winnerPlayer = winnerPlayer;
    emit sig_showWinner();
    //m_mainUi->showWinner(m_winnerPlayer);
}

QGamePlayer *QGameLogic::getWinner()
{
    return m_winnerPlayer;
}

int QGameLogic::getTimeCountDown()
{
    return m_timeCountDown;
}

void QGameLogic::slot_onTimeout()
{
    if(m_timeCountDown)
    {
        emit sig_timeCountDown();
        //m_mainUi->showTimeCountDown(m_timeCountDown);
    }

    m_timeCountDown--;
    if(m_timeCountDown<=0)
    {
        calcWinner();
    }
}
