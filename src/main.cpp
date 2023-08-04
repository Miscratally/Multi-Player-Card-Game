#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create an instance of the MainWindow class, which represents the main application window
    MainWindow w;

    w.setWindowIcon(QIcon(":avator/avator/card_game.ico"));

    //when player close input dialog , then close mainwindow
    if(w.getWillClose())
        return 1;

    // Show the main window to make it visible to the user
    w.show();

    // Start the event loop of the application and wait for user interactions
    return a.exec();
}
