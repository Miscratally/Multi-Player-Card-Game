#ifndef QGAMELOGIC_H
#define QGAMELOGIC_H

#include "qgameplayer.h"
#include "qgamecard.h"
#include <QVector>
#include <QTimer>

const int g_maxPlayerCnt = 5;
const int g_minPlayerCnt = 2;
const int g_numSuit = 4;
const int g_numCard = 13;
const int g_somePlayerNameCnt = 10;
const int g_somePlayerAvatorCnt = 8;
const int g_timeCountDown = 15;

// Forward declaration of MainWindow class
class MainWindow;

class QGameLogic : public QObject
{
    Q_OBJECT
public:
    // Constructor. Initializes the game logic with a pointer to the main UI
    QGameLogic(MainWindow* mainUi);

    // Initializes the game with the specified number of players. Returns true on success
    bool initGame(int playerCount);

    // Starts one round of the game
    void startOneRound();

    // Adds a player to the game at the specified index
    QGamePlayer* addPlayer(int idx);

    // Removes a player from the game
    void removePlayer(int idx);
    void removePlayer(QGamePlayer* player);

    // Finds a player by their name. Returns a pointer to the player if found, else nullptr
    QGamePlayer* findPlayerByName(const QString& name);

    // Returns a vector of all players in the game
    QVector<QGamePlayer*> getAllPlayers();

    // Randomizes the game cards and initializes the card index list
    void randomGameCards();

    // Simulates a player drawing a card
    QGameCard* playerDrawCard(int idx);

    // Returns the total number of cards in the game
    int getCardCnt();

    // Calculates the winner of the current round
    void calcWinner();

    // Returns the winner of the current round
    QGamePlayer* getWinner();

    // Returns the remaining time count down for the current round (in seconds)
    int getTimeCountDown();

public slots:
    // Slot function called when the round timer times out
    void slot_onTimeout();

signals:
    // Signal emitted to update the time countdown in the UI
    void sig_timeCountDown();

    // Signal emitted to show the winner of the current round in the UI
    void sig_showWinner();

protected:
    QVector<QGamePlayer*> m_vectorPlayer; // Vector to store all the game players

    QTimer m_timerRound; // Timer used for one round of the game

    QList<QGameCard*> m_listGameCard; // List to store all the game cards (52 cards)

    QList<int> m_listCardIdx; // List to store the indices of the available cards

    QGamePlayer* m_winnerPlayer = nullptr; // Pointer to the winner of the current round

    MainWindow* m_mainUi = nullptr; // Pointer to the main UI (MainWindow)

    int m_timeCountDown = 0; // Remaining time countdown for the current round (in seconds)
};

#endif // QGAMELOGIC_H
