#include <QDebug>
#include <time.h>
#include <vector>
#include <QString>
#include "qgamelogic.h"
#include "mainwindow.h"

const QString g_somePlayerName[] = {
    QStringLiteral("Jarvan IV"),
    QStringLiteral("Fiora"),
    QStringLiteral("Vayne"),
    QStringLiteral("Morgana"),
    QStringLiteral("Olaf"),
    QStringLiteral("Lucian"),
    QStringLiteral("Taric"),
    QStringLiteral("Katarina"),
    QStringLiteral("Xayah"),
    QStringLiteral("Lillia"),
};

const QString g_somePlayerAvator[] = {
    QStringLiteral("/avator/avator/Avator_0.jpg"),
    QStringLiteral("/avator/avator/Avator_1.jpg"),
    QStringLiteral("/avator/avator/Avator_2.jpg"),
    QStringLiteral("/avator/avator/Avator_3.jpg"),
    QStringLiteral("/avator/avator/Avator_4.jpg"),
    QStringLiteral("/avator/avator/Avator_5.jpg"),
    QStringLiteral("/avator/avator/Avator_6.jpg"),
    QStringLiteral("/avator/avator/Avator_7.jpg"),
    QStringLiteral("/avator/avator/Avator_8.jpg"),
    QStringLiteral("/avator/avator/Avator_9.jpg"),
};

// Constructor
QGameLogic::QGameLogic(MainWindow* mainUi)
    : m_mainUi(mainUi)
{
    // Initialize the player vector
    m_vectorPlayer.resize(GameConstants::MaxPlayerCount);
    for (int i = 0; i < GameConstants::MaxPlayerCount; i++)
        m_vectorPlayer[i] = nullptr;
}

int getRandomPlayerId()
{
    static bool seeded = false; // Use static variable to ensure srand is called only once.
    if (!seeded) {
        srand(time(NULL));
        seeded = true;
    }
    int id = rand() % GameConstants::SomePlayerCount; // Generate a random index within the range of GameConstants::SomePlayerCount.

    return id;
}

bool QGameLogic::initGame(int playerCount)
{
    // Check if the player count within the range.
    if (playerCount < GameConstants::MinPlayerCount || playerCount > GameConstants::MaxPlayerCount)
    {
        return false;
    }

    for(int i = 0; i < playerCount; i++)
    {
        int id = getRandomPlayerId();
        QString name = g_somePlayerName[id];

        // Make sure the generated name is unique among players.
        while(findPlayerByName(name)!=nullptr)
        {
            id = getRandomPlayerId();
            name = g_somePlayerName[id];
        }

        // TODO: Will use same Avator!
        QGamePlayer* player = new QGamePlayer(name, g_somePlayerAvator[id]);
        m_vectorPlayer[i] = player;
    }

    // Initialize all 52 cards.
    for (int suit = 0; suit < GameConstants::NumberOfSuits; suit++)
    {
        for (int number = 0; number < GameConstants::NumberOfCards; number++)
        {
            QGameCard* gameCard = new QGameCard(number, static_cast<EnumSuit>(suit));
            m_listGameCard.append(gameCard); // Add the card to the list of game cards.
        }
    }

    return true;
}

void QGameLogic::startOneRound()
{
    // If the deck is empty, initialize it with random game cards.
    if (getCardCnt() == 0)
    {
        randomGameCards();
    }

    // Clear player cards.
    for (auto& player : m_vectorPlayer)
    {
        // If m_vectorPlayer contains nullptrs, add a check here to avoid dereferencing null pointers.
        if (player)
        {
            player->setGameCard(nullptr);
        }
    }

    // Reset the time countdown to 15 seconds for this round.
    m_timeCountDown = GameConstants::TimeCountDown;
}


// Add a new player to the game.
QGamePlayer* QGameLogic::addPlayer(int idx)
{
    // Check if the index is valid (within the range of players).
    if (idx < 0 || idx >= GameConstants::MaxPlayerCount)
        return nullptr;

    // Check if the player slot is already occupied.
    if (m_vectorPlayer[idx] != nullptr)
        return nullptr;

    // Generate a random id for the player.
    int id = getRandomPlayerId();
    QString name = g_somePlayerName[id];

    // Make sure the generated name is unique among all players.
    while (findPlayerByName(name) != nullptr)
    {
        id = getRandomPlayerId();
        name = g_somePlayerName[id];
    }

    QString avatar = g_somePlayerAvator[id];

    // Create a new QGamePlayer instance with the generated name and avatar.
    QGamePlayer* player = new QGamePlayer(name, avatar);

    // Assign the player to the corresponding slot in the player vector.
    m_vectorPlayer[idx] = player;

    return player;
}


// Remove a player from the vector by index.
void QGameLogic::removePlayer(int idx)
{
    if (m_vectorPlayer[idx] != nullptr)
    {
        delete m_vectorPlayer[idx];
        m_vectorPlayer[idx] = nullptr;
    }
}

void QGameLogic::removePlayer(QGamePlayer *player)
{
    // Attempt to remove the player from the vector.
    if (m_vectorPlayer.removeOne(player))
    {
        delete player;
    }
}

QGamePlayer *QGameLogic::findPlayerByName(const QString &name)
{
    for(int i = 0; i < m_vectorPlayer.length(); i++)
    {
        if(m_vectorPlayer[i])
        {
            if(name == m_vectorPlayer[i]->name())
                return m_vectorPlayer[i];
        }
    }
    return nullptr;
}


QVector<QGamePlayer*> QGameLogic::getAllPlayers()
{
    return m_vectorPlayer;
}

// Shuffle the cards
void QGameLogic::randomGameCards()
{
    int total_cards = GameConstants::NumberOfSuits * GameConstants::NumberOfCards;

    for(int i = 0; i < total_cards; i++)
        m_listCardIdx.append(i);

    for(int i = total_cards - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);

        int temp = m_listCardIdx[i];
        m_listCardIdx[i] = m_listCardIdx[j];
        m_listCardIdx[j] = temp;
    }
}


QGameCard *QGameLogic::playerDrawCard(int idx)
{
    // Check if the index is out of range.
    if (idx < 0 || idx >= GameConstants::MaxPlayerCount)
        return nullptr;

    // Get the player at the specified index.
    QGamePlayer* gamePlayer = m_vectorPlayer[idx];

    // If the player does not exist, return nullptr.
    if (gamePlayer == nullptr)
        return nullptr;

    // If the player already has a card, return the existing card.
    if (gamePlayer->gameCard())
        return gamePlayer->gameCard();

    // Check if there are any cards left in the deck.
    int cardCnt = m_listCardIdx.length();
    // TODO: if there are no cards left, return all cards to the deck, and shuffle the deck.
    if (cardCnt == 0)
        randomGameCards();

    // Get the top card from the deck.
    QGameCard* gameCard = m_listGameCard.at(m_listCardIdx.at(0));
    m_listCardIdx.removeAt(0);

    // Assign the drawn card to the player.
    gamePlayer->setGameCard(gameCard);

    // Check if all players have drawn a card.
    int playerCnt = 0;
    int playerCardCnt = 0;
    for (int i = 0; i < m_vectorPlayer.length(); i++)
    {
        if (m_vectorPlayer[i])
        {
            playerCnt++;
            if (m_vectorPlayer[i]->gameCard())
            {
                playerCardCnt++;
            }
        }
    }

    // Output the number of players and the number of players who have drawn a card.
    qDebug() << "playerCnt =" << playerCnt << ",playerCardCnt =" << playerCardCnt;

    // If all players have drawn a card, calculate the winner.
    if (playerCnt != 0 && playerCnt == playerCardCnt)
    {
        calcWinner();
    }

    // Return the drawn card.
    return gameCard;
}

int QGameLogic::getCardCnt()
{
    return m_listCardIdx.length();
}

// Calculates the winner player based on the game cards held by each player.
void QGameLogic::calcWinner()
{
    QGamePlayer* winnerPlayer = nullptr;

    for (int i = 0; i < m_vectorPlayer.length(); i++)
    {
        // Check if the player exists and has a valid game card.
        if (m_vectorPlayer[i] && m_vectorPlayer[i]->gameCard())
        {
            if (winnerPlayer == nullptr)
            {
                // The first player with a valid game card is considered the winner (temporarily).
                winnerPlayer = m_vectorPlayer[i];
            }
            else
            {
                QGamePlayer* tmpPlayer = m_vectorPlayer[i];

                // Check if the temporary player exists and has a valid game card.
                if (tmpPlayer && tmpPlayer->gameCard())
                {
                    // Compare the game cards to determine the winner.
                    if (winnerPlayer->gameCard()->compare(tmpPlayer->gameCard()) < 0)
                    {
                        winnerPlayer = tmpPlayer;
                    }
                }
            }
        }
    }

    m_winnerPlayer = winnerPlayer;

    // Emit a signal to notify any connected slots that the winner is determined.
    emit sig_showWinner();
}

QGamePlayer *QGameLogic::getWinner()
{
    return m_winnerPlayer;
}

int QGameLogic::getTimeCountDown()
{
    return m_timeCountDown;
}

// This function is called when a timer times out. It is intended to handle game logic related to time countdown.
void QGameLogic::slot_onTimeout()
{
    // If the time countdown is active (greater than 0), emit the signal to notify the UI about the countdown.
    // The UI can then update the displayed countdown timer, if necessary.
    if (m_timeCountDown)
    {
        emit sig_timeCountDown();
    }

    // Decrement the time countdown.
    m_timeCountDown--;

    // If the time countdown has reached zero or less, it's time to calculate the winner of the game.
    if (m_timeCountDown <= 0)
    {
        calcWinner();
    }
}

