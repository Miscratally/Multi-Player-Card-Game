#include "mainwindow.h"
#include "qgamelogic.h"
#include <string>
#include <QDebug>
#include <QSvgRenderer>
#include <QPainter>
#include <QCloseEvent>
#include <QFormLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initWidget();
    initGame();
}

MainWindow::~MainWindow()
{
    delete m_gameLogic;
}

void MainWindow::initWidget()
{
    QWidget* mainWidget = new QWidget();
    this->setCentralWidget(mainWidget);
    this->setMinimumSize(GameConstants::WidgetWidth, GameConstants::WidgetHeight);

    QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);

    {
        QHBoxLayout* hbox = new QHBoxLayout();
        mainLayout->addLayout(hbox);
        {
            QWidget* w = new QWidget();
            hbox->addWidget(w, 1);
        }
        {
            QVBoxLayout* vbox = new QVBoxLayout();
            hbox->addLayout(vbox);

            // Add all UI components here (Vertically)
            {
                // 1.Push button for starting one round
                m_pushBtnStartOneRound = new QPushButton("Start One Round");
                m_pushBtnStartOneRound->setStyleSheet(
                    "QPushButton {"
                    "   font-size: 20pt;"
                    "   font-family: Arial;"
                    "   color: white;"
                    "   background-color: #4CAF50;"     // Green background color
                    "   border-radius: 10px;"
                    "   padding: 30px 50px;"
                    "}"
                    "QPushButton:hover {"
                    "   background-color: #3c9039;"     // Become Dark Green on hover
                    "}"
                    "QPushButton:pressed {"
                    "   background-color: #295f28;"     // Even Darker green on press
                    "}");

                vbox->addWidget(m_pushBtnStartOneRound, 0, Qt::AlignHCenter);

                QObject::connect(m_pushBtnStartOneRound,&QPushButton::clicked,this,&MainWindow::slot_onPushbtnStartOnRoundClicked);
            }

            {
                // 2. Create a horizontal layout to hold the label and remaining card count
                QHBoxLayout* remainingLayout = new QHBoxLayout();

                remainingLayout->addSpacing(50);

                // Label for "Remaining:"
                const QString labelText = u8"Remaining:";
                QLabel* labelRemain = new QLabel(labelText);
                labelRemain->setStyleSheet("QLabel {font: italic 22pt \"Georgia\"; color: #2f4f4f;}");
                remainingLayout->addWidget(labelRemain);

                // Add spacing to separate the label and the remaining card count
                remainingLayout->addSpacing(5);

                // Label to display the remaining card count (initially set to 52)
                const int initialRemainingCards = GameConstants::NumberOfSuits * GameConstants::NumberOfCards;
                m_labelRemainCard = new QLabel(QString::number(initialRemainingCards));
                m_labelRemainCard->setStyleSheet("QLabel {font: italic 22pt \"Georgia\"; color: #2f4f4f;}");
                remainingLayout->addWidget(m_labelRemainCard);

                vbox->addLayout(remainingLayout, Qt::AlignHCenter);
            }

            {
                // 3. Create a Timer and remaining time
                m_labelTimeIcon = new QLabel();
                m_labelTimeIcon->setFixedSize(GameConstants::TimerSize, GameConstants::TimerSize);

                QSvgRenderer renderer(QString(":/avator/avator/timer.svg"));
                QImage image(GameConstants::TimerSize, GameConstants::TimerSize, QImage::Format_ARGB32);
                image.fill(Qt::transparent);
                QPainter painter(&image);
                renderer.render(&painter);

                m_labelTimeIcon->setPixmap(QPixmap::fromImage(image));
                {
                    QHBoxLayout* h = new QHBoxLayout();
                    vbox->addLayout(h);
                    h->addStretch(1);
                    h->addWidget(m_labelTimeIcon);
                    h->addStretch(1);
                }

                m_labelTimeCountDown = new QLabel("15s");
                m_labelTimeCountDown->setStyleSheet("QLabel {font: italic 15pt \"Georgia\"; color: #2f4f4f;}");
                {
                    QHBoxLayout* h = new QHBoxLayout();
                    vbox->addLayout(h);
                    h->addStretch(1);
                    h->addWidget(m_labelTimeCountDown);
                    h->addStretch(1);
                }
            }

            {
                // 4. Create a layout for the winner label and winner text
                QHBoxLayout* hLayout = new QHBoxLayout();
                vbox->addLayout(hLayout);

                // Add stretch on the left to align the winner label to the center
                hLayout->addStretch(1);

                // Create the winner label
                m_labelWinner = new QLabel("");
                m_labelWinner->setStyleSheet("QLabel {font: italic 18pt \"Georgia\"; font-weight: bold; color: red;}");
                hLayout->addWidget(m_labelWinner);
                m_labelWinner->setVisible(false);

                // Create the winner text label
                m_labelWinnerText = new QLabel("Wins The Game!");
                m_labelWinnerText->setStyleSheet("QLabel {font: italic 18pt \"Georgia\"; color: #2f4f4f;}"); // Green color
                hLayout->addWidget(m_labelWinnerText);
                m_labelWinnerText->setVisible(false);

                // Add stretch on the right to align the winner text to the center
                hLayout->addStretch(1);
            }
        }
        {
            QWidget* w = new QWidget();
            hbox->addWidget(w, 1);
        }
    }

    {
        QHBoxLayout* hbox = new QHBoxLayout();
        mainLayout->addLayout(hbox);
        {
            QWidget* w = new QWidget();
            hbox->addWidget(w);
        }
        {
            m_layoutAllPlayerCard = new QHBoxLayout();
            hbox->addLayout(m_layoutAllPlayerCard, 1);
        }
        {
            QWidget* w = new QWidget();
            hbox->addWidget(w);
        }
    }

    {
        QHBoxLayout* hbox = new QHBoxLayout();
        mainLayout->addLayout(hbox);
        {
            QWidget* w = new QWidget();
            hbox->addWidget(w);
        }
        {
            m_layoutAllPlayer = new QHBoxLayout();
            hbox->addLayout(m_layoutAllPlayer, 1);

            for(int i = 0; i < GameConstants::MaxPlayerCount; i++)
            {
                // Create a new player widget for each player index
                QPlayerWidget* playerWidget =new QPlayerWidget(i);

                // Add the player widget to the list and the layout
                m_listPlayerWidget.append(playerWidget);
                m_layoutAllPlayer->addWidget(playerWidget);

                // Connect signals from the player widget to slots in the main window
                QObject::connect(playerWidget, &QPlayerWidget::sig_onPlayerAddOrRemove,
                                 this, &MainWindow::slot_onPlayerWidget_playerAddOrRemove);

                QObject::connect(playerWidget, &QPlayerWidget::sig_onPlayerDrawCard,
                                 this, &MainWindow::slot_onPlayerWidget_playerDrawCard);
            }
        }
        {
            QWidget* w = new QWidget();
            hbox->addWidget(w);
        }
    }
}

void MainWindow::initGame()
{
    // Create the QGameLogic instance and connect signals
    m_gameLogic = new QGameLogic(this);
    connect(m_gameLogic, &QGameLogic::sig_timeCountDown, this, &MainWindow::slot_showTimeCountDown, Qt::QueuedConnection);
    connect(m_gameLogic, &QGameLogic::sig_showWinner, this, &MainWindow::slot_showWinner, Qt::QueuedConnection);

    int playerCnt = GameConstants::MaxPlayerCount;
    // Input player count using QInputDlg
    QInputDlg inputDialog;
    if (inputDialog.exec() == QDialog::Accepted)
    {
        // Get the number of players from the dialog and validate the input
        int num = inputDialog.m_lineEditNum->text().toInt();
        if (num >= GameConstants::MinPlayerCount && num <= GameConstants::MaxPlayerCount)
            playerCnt = num;
    }
    else
    {
        // User clicked the close button or canceled the dialog, set a flag indicating willClose
        this->m_willClose = true;
        return;
    }

    // Initialize the game with the specified number of players
    if (!m_gameLogic->initGame(playerCnt))
    {
        return;
    }

    // Update the player information in the QPlayerWidget
    QVector<QGamePlayer*> vectorPlayer = m_gameLogic->getAllPlayers();
    for (int i = 0; i < vectorPlayer.length(); i++)
    {
        m_listPlayerWidget[i]->setPlayerInfo(vectorPlayer[i]);
    }
}


// End the game and clean up resources
void MainWindow::endGame()
{
    // Step 1: Stop the timer to prevent further timeouts
    m_timer.stop();

    // Step 2: Disconnect the timer's timeout signal from the game logic's slot.
    QObject::disconnect(&m_timer, &QTimer::timeout, m_gameLogic, &QGameLogic::slot_onTimeout);

    // Step 3: Quit the winner calculation thread to stop any ongoing calculations.
    // The thread will continue executing its current task or processing events until it reaches an appropriate point to exit.
    m_threadCalcWinner.quit();

    // Step 4: Wait for the winner calculation thread to finish.
    m_threadCalcWinner.wait();

    // Step 5: Reset the game start flag
    m_gameStarted = false;
}


bool MainWindow::getWillClose()
{
    return m_willClose;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    endGame();
    QMainWindow::closeEvent(event);
}

void MainWindow::slot_showWinner()
{
    // Get the winner player from the game logic
    QGamePlayer* winnerPlayer = m_gameLogic->getWinner();

    // Check if there is a winner or not
    if (winnerPlayer == nullptr)
    {
        m_labelWinner->setText("No one");
    }
    else
    {
        m_labelWinner->setText(winnerPlayer->name());
    }

    // Show the winner label and winner text label
    m_labelWinner->setVisible(true);
    m_labelWinnerText->setVisible(true);

    // End the game
    endGame();
}

// This slot function is called when the time count down signal is emitted by the game logic.
// It updates the label on the main window to show the remaining time in seconds.
void MainWindow::slot_showTimeCountDown()
{
    // Get the remaining time in seconds from the game logic.
    int remainingTimeInSeconds = m_gameLogic->getTimeCountDown();

    QString timeString = QString::number(remainingTimeInSeconds) + "s";

    // Update the label on the main window to display the remaining time.
    m_labelTimeCountDown->setText(timeString);
}

void MainWindow::slot_onPushbtnStartOnRoundClicked()
{
    // Check if the game has already started, if yes, return.
    if (m_gameStarted) {
        return;
    }

    // Set the game as started and update the UI elements accordingly.
    m_gameStarted = true;
    m_labelTimeCountDown->setText("15s");
    m_labelWinner->setVisible(false);
    m_labelWinnerText->setVisible(false);

    // Clear player cards
    for (int i = 0; i < m_listPlayerWidget.length(); i++)
    {
        QPlayerWidget* playerWidget = m_listPlayerWidget[i];
        playerWidget->setCardInfo(nullptr);
    }

    // Configure and start the game timer
    m_timer.setInterval(GameConstants::TimerInterval);

    // Move the m_gameLogic object to the new thread
    m_gameLogic->moveToThread(&m_threadCalcWinner);
    QObject::connect(&m_timer, &QTimer::timeout, m_gameLogic, &QGameLogic::slot_onTimeout);
    m_gameLogic->startOneRound();

    // Start the game logic thread and the timer
    m_threadCalcWinner.start();
    m_timer.start();
}



void MainWindow::slot_onPlayerWidget_playerAddOrRemove(int idx)
{
    // Check if the index is within a valid range
    if (idx >= m_listPlayerWidget.length() || idx < 0)
        return;

    QPlayerWidget* playerWidget = m_listPlayerWidget.at(idx);

    // Check if the player widget is valid
    if (playerWidget == nullptr)
        return;

    if (playerWidget->m_gamePlayer)
    {
        // If the player widget already has a game player associated, remove the player from the game
        m_gameLogic->removePlayer(idx);
        playerWidget->setPlayerInfo(nullptr);
    }
    else
    {
        // If the player widget does not have a game player associated, add a new player to the game
        QGamePlayer* gamePlayer = m_gameLogic->addPlayer(idx);
        playerWidget->setPlayerInfo(gamePlayer);
    }
}


void MainWindow::slot_onPlayerWidget_playerDrawCard(int idx)
{
    // Check whether the game has started or not
    if (!m_gameStarted) {
        // If the game has not started, show a prompt box
        QMessageBox::information(this, "Warning!", "Please start the game first.");
        return;
    }

    // Check if the given index is valid
    if (idx < 0 || idx >= m_listPlayerWidget.length()) {
        return;
    }

    QPlayerWidget* playerWidget = m_listPlayerWidget.at(idx);

    // Check if the player widget is valid and has a game player
    if (playerWidget == nullptr || playerWidget->m_gamePlayer == nullptr) {
        return;
    }

    QGameCard* gameCard = m_gameLogic->playerDrawCard(idx);

    if (gameCard == nullptr) {
        return;
    }

    // Update the player widget to display the drawn card
    playerWidget->setCardInfo(gameCard);

    // Update the remaining card count label
    int cardCnt = m_gameLogic->getCardCnt();
    m_labelRemainCard->setText(QString::number(cardCnt));
}

QPlayerWidget::QPlayerWidget(int idx)
    : m_idx(idx)
    , m_gamePlayer(nullptr)
{
    initWidget();

    // Connect the clicked signal of m_pushBtnDrawCard to slot_onDrawCardPushBtnClicked
    QObject::connect(this->m_pushBtnDrawCard, &QPushButton::clicked, this, &QPlayerWidget::slot_onDrawCardPushBtnClicked);

    // Connect the clicked signal of m_pushBtnAddOrRemove to slot_onAddOrRemovePushBtnClicked
    QObject::connect(this->m_pushBtnAddOrRemove, &QPushButton::clicked, this, &QPlayerWidget::slot_onAddOrRemovePushBtnClicked);
}

void QPlayerWidget::initWidget()
{
    // Create the main layout for the widget
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Label to display the card number and suit
    m_labelCardNumSuit = new QLabel("");

    m_labelCardNumSuit->setStyleSheet("font: italic 22pt \"Georgia\"");

    // Button to draw a card
    m_pushBtnDrawCard = new QPushButton("Draw");
    m_pushBtnDrawCard->setStyleSheet(
        "QPushButton {"
        "   font-size: 15pt;"
        "   font-family: Arial;"
        "   color: white;"
        "   background-color: #4CAF50;"     // Green background color
        "   padding: 1px 1px;"
        "   min-width: 5px;"
        "   min-height: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #3c9039;"     // Become Dark Green on hover
        "}"
        "QPushButton:pressed {"
        "   background-color: #295f28;"     // Even Darker green on press
        "}");

    // Label to display the player's name
    m_labelName = new QLabel("   ");
    m_labelName->setStyleSheet("font: 15pt Arial;");

    // Label to display the player's avatar (image)
    m_labelAvator = new QLabel("");
    m_labelAvator->setStyleSheet("border-image: url(:/avator/avator/def_head.png);");
    m_labelAvator->setFixedSize(QSize(GameConstants::AvatorSize, GameConstants::AvatorSize));

    // Button to add or remove the player
    m_pushBtnAddOrRemove = new QPushButton("Add");
    m_pushBtnAddOrRemove->setStyleSheet(
        "QPushButton {"
        "   font-size: 15pt;"
        "   font-family: Arial;"
        "   color: white;"
        "   background-color: #4CAF50;"     // Green background color
        "   border-radius: 10px;"
        "   padding: 1px 1px;"
        "   min-width: 5px;"
        "   min-height: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #3c9039;"     // Become Dark Green on hover
        "}"
        "QPushButton:pressed {"
        "   background-color: #295f28;"     // Even Darker green on press
        "}");

    // Create a form layout to arrange the widgets
    QFormLayout* formLayout = new QFormLayout();
    formLayout->setContentsMargins(10, 10, 10, 10);
    formLayout->setSpacing(10);
    formLayout->setAlignment(Qt::AlignHCenter);

    // Add each row into the Layout
    formLayout->addRow("Card:", m_labelCardNumSuit);
    formLayout->addRow(m_pushBtnDrawCard);
    formLayout->addRow("Name:", m_labelName);
    formLayout->addRow(" ", m_labelAvator);
    formLayout->addRow(m_pushBtnAddOrRemove);

    // Add the form layout to the main layout
    mainLayout->addLayout(formLayout);
    mainLayout->setContentsMargins(30, 20, 30, 20);
    mainLayout->setSpacing(30);
}

void QPlayerWidget::setPlayerInfo(const QGamePlayer *gamePlayer)
{
    // If gamePlayer is nullptr, it means the player is not present.
    if (gamePlayer == nullptr)
    {
        m_gamePlayer = nullptr;
        m_labelName->setText("");               // Clear player name
        m_labelAvator->setStyleSheet("QLabel {border-image: url(:/avator/avator/def_head.png);}"); // Set default avatar
        m_pushBtnAddOrRemove->setText("Add");   // Change button text to "Add"
        m_pushBtnAddOrRemove->setStyleSheet(
            "QPushButton {"
            "   font-size: 15pt;"
            "   font-family: Arial;"
            "   color: white;"
            "   background-color: #4CAF50;"     // Green background color
            "   border-radius: 10px;"
            "   padding: 1px 1px;"
            "   min-width: 5px;"
            "   min-height: 10px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #3c9039;"     // Become Dark Green on hover
            "}"
            "QPushButton:pressed {"
            "   background-color: #295f28;"     // Even Darker green on press
            "}");

        m_labelCardNumSuit->setText("");        // Clear player card information
        return;
    }

    // If gamePlayer is not nullptr, it means the player is present.
    m_gamePlayer = gamePlayer;
    m_labelName->setText(gamePlayer->name());   // Set player name
    QString qss = QString("QLabel {border-image: url(:%1);}").arg(gamePlayer->avator());
    m_labelAvator->setStyleSheet(qss);          // Set player's avatar using the URL provided by gamePlayer

    m_pushBtnAddOrRemove->setText("Remove");    // Change button text to "Remove"
    m_pushBtnAddOrRemove->setStyleSheet(
        "QPushButton {"
        "   font-size: 15pt;"
        "   font-family: Arial;"
        "   color: white;"
        "   background-color: #FF0000;"         // Red background color
        "   border-radius: 10px;"
        "   padding: 1px 1px;"
        "   min-width: 5px;"
        "   min-height: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #B22222;"         // Become Dark Red on hover
        "}"
        "QPushButton:pressed {"
        "   background-color: #8B0000;"         // Even Darker Red on press
        "}");
}


void QPlayerWidget::setCardInfo(const QGameCard *gameCard)
{
    if(gameCard==nullptr)
    {
        m_labelCardNumSuit->setText("");
        return;
    }
    // Set card string context
    m_labelCardNumSuit->setText(gameCard->cardString());

    // Check the suit of this card
    EnumSuit cardSuit = gameCard->suit();

    // Set the color of card
    if (cardSuit == EnumSuit::Heart || cardSuit == EnumSuit::Diamond)
    {
        m_labelCardNumSuit->setStyleSheet("font: italic 22pt \"Georgia\"; color: red;");
    } else {
        m_labelCardNumSuit->setStyleSheet("font: italic 22pt \"Georgia\";");
    }
}


void QPlayerWidget::slot_onAddOrRemovePushBtnClicked()
{
    emit sig_onPlayerAddOrRemove(m_idx);
}

void QPlayerWidget::slot_onDrawCardPushBtnClicked()
{
    emit sig_onPlayerDrawCard(m_idx);
}


