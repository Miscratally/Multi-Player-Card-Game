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
    bool getWillClose();
    virtual void closeEvent(QCloseEvent *event) override;
	
public slots:
    void slot_showWinner();
    void slot_showTimeCountDown();
    void slot_onPushbtnStartOnRoundClicked();
    void slot_onPlayerWidget_playerAddOrRemove(int idx);
    void slot_onPlayerWidget_playerDrawCard(int idx);

protected:
    QPointer<QPushButton>  m_pushBtnStartOneRound;  // Start One Round Button
    QPointer<QLabel> m_labelRemainCard;             // Remaining cards
    QPointer<QLabel> m_labelTimeIcon;               // Timer Icon
    QPointer<QLabel> m_labelTimeCountDown;          // Time Count Down
    QPointer<QLabel> m_labelWinner;                 // Winner Name Label
    QPointer<QLabel> m_labelWinnerText;             // Winner Text Label
    QPointer<QHBoxLayout> m_layoutAllPlayerCard;    // All Player Cards
    QPointer<QHBoxLayout> m_layoutAllPlayer;        // All Players

    QList<QPointer<QPlayerWidget>> m_listPlayerWidget;

    QTimer m_timer;
    QThread m_threadCalcWinner;                     // Thread on timmer to calcWinner
    QGameLogic* m_gameLogic;
    bool m_willClose = false;
    bool m_gameStarted = false;                     // Status of the game
};
#endif // MAINWINDOW_H


class QPlayerWidget : public QWidget
{
    Q_OBJECT
public:
    // Constructor - Takes the player index as input.
    explicit QPlayerWidget(int idx);

    // Initializes the widgets and layout for the player.
    void initWidget();

    void setPlayerInfo(const QGamePlayer* gamePlayer);

    void setCardInfo(const QGameCard* gameCard);

signals:
    // Signal emitted when the player add or remove button is clicked.
    void sig_onPlayerAddOrRemove(int idx);

    // Signal emitted when the player draw card button is clicked.
    void sig_onPlayerDrawCard(int idx);

public slots:
    // Slot to handle the add or remove button click.
    void slot_onAddOrRemovePushBtnClicked();

    // Slot to handle the draw card button click.
    void slot_onDrawCardPushBtnClicked();

public:
    int m_idx;  // Player index.

    // Widgets for player information display.
    QPointer<QLabel> m_labelCardNumSuit;   // Card number and suit.
    QPointer<QPushButton> m_pushBtnDrawCard;   // Draw card button.
    QPointer<QLabel> m_labelName;   // Player name.
    QPointer<QLabel> m_labelAvator;   // Player avatar.
    QPointer<QPushButton> m_pushBtnAddOrRemove;   // Add or remove player button.

    const QGamePlayer* m_gamePlayer;   // Pointer to the game player information.
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

        // Creating the first horizontal layout for the input label and line edit
        {
            QHBoxLayout* hbox = new QHBoxLayout();
            mainLayout->addLayout(hbox);

            // Set label
            QLabel* label = new QLabel("Number of players:");
            label->setStyleSheet("QLabel {"
                                 "    font-size: 16px; "
                                 "    font-family: 'Arial'; "
                                 "    font-weight: bold; "
                                 "    background-color: #F0F0F0; "
                                 "    padding: 2px; "
                                 "    border-radius: 5px; "
                                 "}");

            hbox->addWidget(label);

            // Set Input text label
            m_lineEditNum->setPlaceholderText("Enter a number between 2 and 5");
            hbox->addWidget(m_lineEditNum);
        }

        // Creating the second horizontal layout for the Start button
        {
            QHBoxLayout* hbox = new QHBoxLayout();
            mainLayout->addLayout(hbox);

            hbox->addWidget(m_pushBtnStart);

            // Connecting the Start button's clicked signal to the accept slot of the dialog
            connect(m_pushBtnStart, &QPushButton::clicked, this, &QInputDlg::onStartClicked);
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
    void onStartClicked()
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


