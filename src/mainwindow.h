#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QCoreApplication>
#include <QPointer>
#include <QCloseEvent>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QThread>
#include <QLineEdit>
#include <QDialog>

#include "qgamecard.h"
#include "qconstants.h"
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
    QPointer<QPushButton>  m_pushBtnStartOneRound;
    QPointer<QLabel> m_labelRemainCard;         // Remain card
    QPointer<QLabel> m_labelTimeIcon;           //
    QPointer<QLabel> m_labelTimeCountDown;      // Time count down
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
    // Constructor for the dialog with initialization list
    QInputDlg() : m_lineEditNum(new QLineEdit), m_pushBtnStart(new QPushButton("Start The Game"))
    {
        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        setWindowTitle("Multi-Player Card Game"); // Set the window title
        QFont font;

        // Creating the first horizontal layout for the input label and line edit
        {
            QHBoxLayout* hbox = new QHBoxLayout();
            mainLayout->addLayout(hbox);

            // Set font
            font.setPointSize(10);
            font.setBold(true);      // 设置字体为粗体

            // Set label
            QLabel* label = new QLabel("Number of players:");
            label->setFont(font);

            hbox->addWidget(label);

            m_lineEditNum->setPlaceholderText("Enter a number between 2 and 5");
            hbox->addWidget(m_lineEditNum);
        }

        // Creating the second horizontal layout for the Start button
        {
            QHBoxLayout* hbox = new QHBoxLayout();
            mainLayout->addLayout(hbox);

            hbox->addWidget(m_pushBtnStart);

            // Connecting the Start button's clicked signal to the accept slot of the dialog
            connect(m_pushBtnStart, &QPushButton::clicked, this, &QInputDlg::onOkClicked);
        }

        // Set the dialog's size and style sheet for better appearance
        setFixedSize(GameConstants::WindowWidth, GameConstants::WindowHeight);
        setStyleSheet(
            "QDialog { background-color: white; border: 2px solid black; border-radius: 5px; }"
            "QLineEdit { font: 12pt Arial; padding: 5px; border: 1px solid gray; border-radius: 5px; }"
            "QPushButton { font: 12pt Arial; padding: 5px 10px; border: 2px solid black; border-radius: 5px; }"
            "QPushButton:hover { background-color: lightgray; }"
            );
    }

private slots:
    // Slot to handle the OK button click
    void onOkClicked()
    {
        // Validate user input and accept the dialog if valid
        int num = m_lineEditNum->text().toInt();
        if (num >= GameConstants::MinPlayerCount && num <= GameConstants::MaxPlayerCount)
        {
            accept();
        }
        else
        {
            // Show an error message if the input is invalid
            QMessageBox::critical(this, "Error", "Invalid input. Please enter a number between 2 and 5.");
        }
    }

protected:
    void closeEvent(QCloseEvent *event) override
    {
        // Exit the game
        QCoreApplication::quit();
        event->accept();
    }

public:
    QPointer<QLineEdit> m_lineEditNum;
    QPointer<QPushButton> m_pushBtnStart;
};


