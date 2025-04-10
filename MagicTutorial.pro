QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ability.cpp \
    card.cpp \
    cardapimanager.cpp \
    carddictionary.cpp \
    command.cpp \
<<<<<<< HEAD
    deck.cpp \
    gameboard.cpp \
=======
>>>>>>> 0fc46d9bb4f76071fbb709f34ddd6c8484df1181
    gamemanager.cpp \
    gamestate.cpp \
    main.cpp \
    mainwindow.cpp \
    player.cpp \
    type.cpp \
    zone.cpp

HEADERS += \
    ability.h \
    card.h \
    cardapimanager.h \
    carddictionary.h \
    command.h \
<<<<<<< HEAD
    deck.h \
    gameboard.h \
=======
>>>>>>> 0fc46d9bb4f76071fbb709f34ddd6c8484df1181
    gamemanager.h \
    gamestate.h \
    mainwindow.h \
    phase.h \
    player.h \
    type.h \
    zone.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
