#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPointer>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QThread>
#include <QLineEdit>
#include <QDialog>

#include "qgamecard.h"
#include "qgameplayer.h"

class QPlayerWidget;
class QGameLogic;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initWidget();
    void initSignals();

    void initGame();
    void endGame();
public slots:
    void slot_showWinner();
    void slot_showTimeCountDown();
    void slot_onPushbtnStartOnRoundClicked();
    void slot_onPlayerWidget_playerAddOrRemove(int idx);
    void slog_onPlayerWidget_playerDrawCard(int idx);

protected:
    QPointer<QPushButton> m_pushbtnStartOneRound;
    QPointer<QLabel> m_labelRemainCard;         //remain card
    QPointer<QLabel> m_labelTimeIcon;           //
    QPointer<QLabel> m_labelTimeCountDown;      //time count down
    QPointer<QLabel> m_labelWinner;             // winner
    QPointer<QLabel> m_labelWinnerText;             // winner
    QPointer<QHBoxLayout> m_layoutAllPlayerCard;//all player card
    QPointer<QHBoxLayout> m_layoutAllPlayer;    //all player

    QList<QPointer<QPlayerWidget>> m_listPlayerWidget;

    QTimer m_timer;
    QThread m_threadCalcWinner;             //thread on timmer  calcWinner
    QGameLogic* m_gameLogic;
};
#endif // MAINWINDOW_H


class QPlayerWidget: public QWidget
{
    Q_OBJECT
public:
    QPlayerWidget(int idx);
    void initWidget();
    void setPlayerInfo(const QGamePlayer* gamePlayer);
    void setCardInfo(const QGameCard* gameCard);
signals:
    void sig_onPlayerAddOrRemove(int idx);
    void sig_onPlayerDrawCard(int idx);

public slots:
    void slot_onAddOrRemovePushBtnClicked();
    void slot_onDrawCardPushBtnClicked();
public:
    int m_idx;
    QPointer<QLabel> m_labelCardNumSuit;        //card num suit
    QPointer<QPushButton> m_pushbtnDrawCard;    //draw card
    QPointer<QLabel> m_labelName;               //player Name
    QPointer<QLabel> m_labelAvator;             //player avator
    QPointer<QPushButton> m_pushbtnAddOrRemove;
    const QGamePlayer* m_gamePlayer;
};

class QInputDlg : public QDialog
{
    Q_OBJECT
public:
    QInputDlg()
    {
        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        {
            QHBoxLayout* hbox = new QHBoxLayout();
            mainLayout->addLayout(hbox);

            hbox->addWidget(new QLabel("input player number:[2, 5]"));
            m_lineeditNum = new QLineEdit();
            hbox->addWidget(m_lineeditNum);
        }
        {
            QHBoxLayout* hbox = new QHBoxLayout();
            mainLayout->addLayout(hbox);

            m_pushbtnOk = new QPushButton("OK");
            hbox->addWidget(m_pushbtnOk);

            QObject::connect(m_pushbtnOk,&QPushButton::clicked,this, &QInputDlg::accept);
        }
    }
public:
    QPointer<QLineEdit> m_lineeditNum;
    QPointer<QPushButton> m_pushbtnOk;
};
