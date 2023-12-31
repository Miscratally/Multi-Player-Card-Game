QT += core gui
QT += svg

C_ICONS = card_game.ico

RC_ICONS = avator/card_game.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qgamecard.cpp \
    qgamelogic.cpp \
    qgameplayer.cpp

HEADERS += \
    mainwindow.h \
    qconstants.h \
    qgamecard.h \
    qgamelogic.h \
    qgameplayer.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    game.qrc
