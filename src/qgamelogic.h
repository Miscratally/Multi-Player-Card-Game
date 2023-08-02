#ifndef QGAMELOGIC_H
#define QGAMELOGIC_H

#include "qgameplayer.h"
#include "qgamecard.h"
#include <QVector>
#include <QTimer>

const int g_maxPlayerCnt = 5;

class MainWindow;
class QGameLogic : public QObject
{
    Q_OBJECT
public:
    QGameLogic(MainWindow* mainUi);
    bool initGame(int playerCount);

    void startOneRound();

    QGamePlayer* addPlayer(int idx);
    void removePlayer(int idx);
    void removePlayer(QGamePlayer* player);

    QGamePlayer* findPlayerByName(const QString& name);

    QVector<QGamePlayer*> getAllPlayers();

    void randomGameCards();
    QGameCard* playerDrawCard(int idx);

    int getCardCnt();
    void calcWinner();
    QGamePlayer* getWinner();
    int getTimeCountDown();
public slots:
    void slot_onTimeout();
signals:
    void sig_timeCountDown();
    void sig_showWinner();
protected:
    QVector<QGamePlayer*> m_vectorPlayer;

    QTimer m_timerRound;    //timer in one round

    QList<QGameCard*> m_listGameCard;   //52 cards
    QList<int>  m_listCardIdx;          //card idx;

    QGamePlayer* m_winnerPlayer = nullptr;
    MainWindow* m_mainUi =nullptr;
    int m_timeCountDown =0;
};

#endif // QGAMELOGIC_H
