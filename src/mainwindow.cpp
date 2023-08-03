#include "mainwindow.h"
#include "qgamelogic.h"
#include <QSvgRenderer>
#include <QPainter>

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
    this->setMinimumSize(1324,768);

    QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);
    /*{
        QHBoxLayout* hbox = new QHBoxLayout();
        mainLayout->addLayout(hbox);
        {
            QWidget* w = new QWidget();
            hbox->addWidget(w,1);
        }
        {
            QVBoxLayout* vbox = new QVBoxLayout();
            hbox->addLayout(vbox);
        }
        {
            QWidget* w = new QWidget();
            hbox->addWidget(w,1);
        }
    }
*/
    {
        QHBoxLayout* hbox = new QHBoxLayout();
        mainLayout->addLayout(hbox);
        {
            QWidget* w = new QWidget();
            hbox->addWidget(w,1);
        }
        {
            QVBoxLayout* vbox = new QVBoxLayout();
            hbox->addLayout(vbox);
            {
                m_pushBtnStartOneRound = new QPushButton(u8"Start One Round");
                m_pushBtnStartOneRound->setStyleSheet("QPushButton {font: 20pt \"Arial\";}");
                vbox->addWidget(m_pushBtnStartOneRound,Qt::AlignHCenter);
                QObject::connect(m_pushBtnStartOneRound,&QPushButton::clicked,this,&MainWindow::slot_onPushbtnStartOnRoundClicked);
            }
            {
                QHBoxLayout* h = new QHBoxLayout();
                vbox->addLayout(h,Qt::AlignHCenter);

                QLabel* labelRemain = new QLabel(u8"Remaining:");
                labelRemain->setStyleSheet("QLabel {font: 20pt \"Arial\";}");
                h->addWidget(labelRemain);

                m_labelRemainCard = new QLabel("52");
                m_labelRemainCard->setStyleSheet("QLabel {font: 20pt \"Arial\";}");
                h->addWidget(m_labelRemainCard);
            }
            {
                m_labelTimeIcon = new QLabel();
                m_labelTimeIcon->setFixedSize(50,50);
                QSvgRenderer renderer(QString(":/avator/avator/timer.svg"));
                QImage image(50, 50, QImage::Format_ARGB32);
                image.fill(Qt::transparent);
                // 创建一个QPainter对象，用于渲染SVG图像到QImage中
                QPainter painter(&image);
                // 使用QSvgRenderer绘制SVG图像到QImage中
                renderer.render(&painter);
                // 将QImage设置为QLabel的图像
                m_labelTimeIcon->setPixmap(QPixmap::fromImage(image));
                {
                    QHBoxLayout* h = new QHBoxLayout();
                    vbox->addLayout(h);
                    h->addStretch(1);
                    h->addWidget(m_labelTimeIcon);
                    h->addStretch(1);
                }

                m_labelTimeCountDown = new QLabel("15s");
                m_labelTimeCountDown->setStyleSheet("QLabel {font: 15pt \"Arial\"; color: red;}");
                {
                    QHBoxLayout* h = new QHBoxLayout();
                    vbox->addLayout(h);
                    h->addStretch(1);
                    h->addWidget(m_labelTimeCountDown);
                    h->addStretch(1);
                }
            }

            {
                QHBoxLayout* h  = new QHBoxLayout();
                h->addStretch(1);
                vbox->addLayout(h);
                m_labelWinner = new QLabel("");
                m_labelWinner->setStyleSheet("QLabel {font: 20pt \"Arial\"; color: red; font-weight: bold;}");
                h->addWidget(m_labelWinner);
                m_labelWinner->setVisible(false);

                m_labelWinnerText =new QLabel("is Winner!");
                m_labelWinnerText->setStyleSheet("QLabel {font: 15pt \"Arial\";}");
                h->addWidget(m_labelWinnerText);
                m_labelWinnerText->setVisible(false);
                h->addStretch(1);
            }
        }
        {
            QWidget* w = new QWidget();
            hbox->addWidget(w,1);
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
            hbox->addLayout(m_layoutAllPlayerCard,1);
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
            hbox->addLayout(m_layoutAllPlayer,1);
            for(int i =0;i<GameConstants::MaxPlayerCount;i++)
            {
                QPlayerWidget* playerWidget =new QPlayerWidget(i);

                m_listPlayerWidget.append(playerWidget);

                m_layoutAllPlayer->addWidget(playerWidget);

                QObject::connect(playerWidget,&QPlayerWidget::sig_onPlayerAddOrRemove,
                                 this,&MainWindow::slot_onPlayerWidget_playerAddOrRemove);

                QObject::connect(playerWidget,&QPlayerWidget::sig_onPlayerDrawCard,
                                 this,&MainWindow::slog_onPlayerWidget_playerDrawCard);
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
    m_gameLogic = new QGameLogic(this);
    QObject::connect(m_gameLogic,&QGameLogic::sig_timeCountDown,this,&MainWindow::slot_showTimeCountDown,Qt::QueuedConnection);
    QObject::connect(m_gameLogic,&QGameLogic::sig_showWinner,this,&MainWindow::slot_showWinner,Qt::QueuedConnection);

    int playerCnt = 5;
    //input player cnt;
    QInputDlg inputDialog;
    if(inputDialog.exec()==QDialog::Accepted)
    {
        int num = inputDialog.m_lineEditNum->text().toInt();
        if(num >=2&&num<=5)
            playerCnt = num;
    }

    if(m_gameLogic->initGame(playerCnt)==false)
        return;

    QVector<QGamePlayer*> vectorPlayer = m_gameLogic->getAllPlayers();

    for(int i =0;i<vectorPlayer.length();i++)
    {
        m_listPlayerWidget[i]->setPlayerInfo(vectorPlayer[i]);
    }

}

void MainWindow::endGame()
{
    m_timer.stop();

    QObject::disconnect(&m_timer,&QTimer::timeout,m_gameLogic,&QGameLogic::slot_onTimeout);
    m_threadCalcWinner.quit();
    m_threadCalcWinner.wait();
}

void MainWindow::slot_showWinner()
{
    QGamePlayer* winnerPlayer = m_gameLogic->getWinner();
    m_labelWinner->setText(winnerPlayer->name());
    m_labelWinner->setVisible(true);
    m_labelWinnerText->setVisible(true);
    endGame();
}

void MainWindow::slot_showTimeCountDown()
{
    int timeCountDown = m_gameLogic->getTimeCountDown();
    m_labelTimeCountDown->setText(QString::number(m_gameLogic->getTimeCountDown())+ QString("s"));
}

void MainWindow::slot_onPushbtnStartOnRoundClicked()
{
    m_labelWinner->setVisible(false);
    m_labelWinnerText->setVisible(false);
    //clear player card
    for(int i = 0;i<m_listPlayerWidget.length();i++)
    {
        QPlayerWidget* playerWidget = m_listPlayerWidget[i];
        playerWidget->setCardInfo(nullptr);
    }

    //start one round
    m_timer.setInterval(1000);
    m_gameLogic->moveToThread(&m_threadCalcWinner);
    QObject::connect(&m_timer,&QTimer::timeout,m_gameLogic,&QGameLogic::slot_onTimeout);
    m_gameLogic->startOneRound();

    m_threadCalcWinner.start();
    m_timer.start();
}


void MainWindow::slot_onPlayerWidget_playerAddOrRemove(int idx)
{
    if(idx >=m_listPlayerWidget.length()||idx<0)
        return;

    QPlayerWidget* playerWidget = m_listPlayerWidget.at(idx);
    if(playerWidget ==nullptr)
        return;

    if(playerWidget->m_gamePlayer)
    {
        m_gameLogic->removePlayer(idx);
        playerWidget->setPlayerInfo(nullptr);
    }
    else
    {
        QGamePlayer* gamePlayer = m_gameLogic->addPlayer(idx);
        playerWidget->setPlayerInfo(gamePlayer);
    }
}

void MainWindow::slog_onPlayerWidget_playerDrawCard(int idx)
{
    if(idx >=m_listPlayerWidget.length()||idx<0)
        return;

    QPlayerWidget* playerWidget = m_listPlayerWidget.at(idx);
    if(playerWidget ==nullptr)
        return;

    if(playerWidget->m_gamePlayer ==nullptr)
        return;

    QGameCard* gameCard = m_gameLogic->playerDrawCard(idx);
    if(gameCard==nullptr)
        return;
    playerWidget->setCardInfo(gameCard);

    int cardCnt = m_gameLogic->getCardCnt();
    m_labelRemainCard->setText(QString::number(cardCnt));
}

QPlayerWidget::QPlayerWidget(int idx)
    :m_idx(idx)
    ,m_gamePlayer(nullptr)
{
    initWidget();

    QObject::connect(this->m_pushbtnDrawCard,&QPushButton::clicked,this,&QPlayerWidget::slot_onDrawCardPushBtnClicked);
    QObject::connect(this->m_pushbtnAddOrRemove,&QPushButton::clicked,this,&QPlayerWidget::slot_onAddOrRemovePushBtnClicked);
}

void QPlayerWidget::initWidget()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    m_labelCardNumSuit = new QLabel("");
    m_labelCardNumSuit->setStyleSheet("QLabel {font: 25pt \"Arial\";}");
    mainLayout->addWidget(m_labelCardNumSuit,0,Qt::AlignHCenter);

    m_pushbtnDrawCard = new QPushButton("Draw");
    m_pushbtnDrawCard->setStyleSheet("QPushButton {font: 15pt \"Arial\";}");
    mainLayout->addWidget(m_pushbtnDrawCard,0,Qt::AlignHCenter);

    m_labelName = new QLabel("Name");
    m_labelName->setStyleSheet("QLabel {font: 15pt \"Arial\";}");
    mainLayout->addWidget(m_labelName,0,Qt::AlignHCenter);

    m_labelAvator = new QLabel("");
    m_labelAvator->setStyleSheet("QLabel {border-image: url(:/avator/avator/def_head.png);}");
    m_labelAvator->setFixedSize(QSize(100,100));
    mainLayout->addWidget(m_labelAvator,0,Qt::AlignHCenter);

    m_pushbtnAddOrRemove = new QPushButton("Add Player");
    m_pushbtnAddOrRemove->setStyleSheet("QPushButton {font: 15pt \"Arial\";}");
    mainLayout->addWidget(m_pushbtnAddOrRemove,0,Qt::AlignHCenter);

}

void QPlayerWidget::setPlayerInfo(const QGamePlayer *gamePlayer)
{
    if(gamePlayer ==nullptr)
    {
        m_gamePlayer = nullptr;
        m_labelName->setText("Name");
        m_labelAvator->setStyleSheet("QLabel {border-image: url(:/avator/avator/def_head.png);}");
        m_pushbtnAddOrRemove->setText("Add Player");

        m_labelCardNumSuit->setText("");
        return;
    }

    m_gamePlayer = gamePlayer;
    m_labelName->setText(gamePlayer->name());
    QString qss = QString("QLabel {border-image: url(:%1);}").arg(gamePlayer->avator());
    m_labelAvator->setStyleSheet(qss);

    m_pushbtnAddOrRemove->setText("Remove Player");
}

void QPlayerWidget::setCardInfo(const QGameCard *gameCard)
{
    if(gameCard==nullptr)
    {
        m_labelCardNumSuit->setText("");
        return;
    }

    QString str;
    str = gameCard->cardString();
    m_labelCardNumSuit->setText(str);
}

void QPlayerWidget::slot_onAddOrRemovePushBtnClicked()
{
    emit sig_onPlayerAddOrRemove(m_idx);
}

void QPlayerWidget::slot_onDrawCardPushBtnClicked()
{
    emit sig_onPlayerDrawCard(m_idx);
}


