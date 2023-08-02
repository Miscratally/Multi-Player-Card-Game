#include "qgamelogic.h"
#include "mainwindow.h"
#include <QDebug>


const QString  g_somePlayerName[10]={
    {"Alice"},              //0
    {"Box"},
    {"Car"},
    {"Paul"},
    {"John"},
    {"Jusin"},
    {"Messi"},
    {"Mask"},
    {"Frank"},
    {"God"},                //9
};

const int g_somePlayerNameCnt = 10;

const QString  g_somePlayerAvator[9]={
    {"/avator/avator/Anivia_Square_0.png"},        //0
    {"/avator/avator/Annie_square_0.png"},
    {"/avator/avator/Vayne_Square_0.png"},
    {"/avator/avator/Renekton_Square_0.png"},
    {"/avator/avator/MissFortune_square_0.png"},
    {"/avator/avator/Jinx_Square_0.png"},
    {"/avator/avator/Galio_square_0.png"},
    {"/avator/avator/Ashe_square_0.png"},         //7
};

const int g_somePlayerAvatorCnt = 8;
#include <time.h>
QString getRandName()
{
    srand(time(NULL));
    int idx =rand()%g_somePlayerNameCnt;

    return g_somePlayerName[idx];
}

QString getRandAvator()
{
    srand(time(NULL));
    int idx =rand()%g_somePlayerAvatorCnt;

    return g_somePlayerAvator[idx];
}

QGameLogic::QGameLogic(MainWindow* mainUi)
    :m_mainUi(mainUi)
{
    //resize
    m_vectorPlayer.resize(g_maxPlayerCnt);
    for(int i = 0;i<g_maxPlayerCnt;i++)
        m_vectorPlayer[i] = nullptr;
}

bool QGameLogic::initGame(int playerCount)
{
    if(playerCount >g_maxPlayerCnt)
    {
        return false;
    }

    for(int i =0;i<playerCount;i++)
    {
        QString name = getRandName();
        while(findPlayerByName(name)!=nullptr)
        {
            name = getRandName();
        }

        QGamePlayer* player = new QGamePlayer(name,getRandAvator());
        m_vectorPlayer[i] = player;
    }


    //init gamecard
    for(int i = 0;i<4;i++)
    {
        for(int j = 0;j<13;j++)
        {
            QGameCard* gameCard = new QGameCard(j,(EnumSuit)i);//number,suit
            m_listGameCard.append(gameCard);
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
            m_vectorPlayer[i]->m_gameCard = nullptr;
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
            if(m_vectorPlayer[i]->m_name==name)
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

    if(gamePlayer->m_gameCard)
        return gamePlayer->m_gameCard;

    int cardCnt = m_listCardIdx.length();
    if(cardCnt ==0)
        return nullptr;

    QGameCard* gameCard = m_listGameCard.at(m_listCardIdx.at(0));
    m_listCardIdx.removeAt(0);

    gamePlayer->setCard(gameCard);

    //if all player draw card
    int playerCnt = 0;
    int playerCardCnt = 0;
    for(int i = 0;i<m_vectorPlayer.length();i++)
    {
        if(m_vectorPlayer[i])
        {
            playerCnt++;
            if(m_vectorPlayer[i]->m_gameCard)
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
        if(m_vectorPlayer[i]&&m_vectorPlayer[i]->m_gameCard)
        {
            if(winnerPlayer==nullptr)
            {
                winnerPlayer = m_vectorPlayer[i];
            }
            else
            {
                QGamePlayer* tmpPlayer = m_vectorPlayer[i];
                if(tmpPlayer&&tmpPlayer->m_gameCard)
                {
                    //calc win
                    if(winnerPlayer->m_gameCard->compare(tmpPlayer->m_gameCard)<0)
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
