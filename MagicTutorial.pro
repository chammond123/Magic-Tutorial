QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ability.cpp \
    bot.cpp \
    card.cpp \
    cardapimanager.cpp \
    cardbutton.cpp \
    carddictionary.cpp \
    command.cpp \
    deck.cpp \
    gamemanager.cpp \
    gamestate.cpp \
    main.cpp \
    mainwindow.cpp \
    manacollection.cpp \
    player.cpp \
    scenebot.cpp \
    textparser.cpp \
    type.cpp \
    zone.cpp

HEADERS += \
    ability.h \
    bot.h \
    card.h \
    cardapimanager.h \
    cardbutton.h \
    carddictionary.h \
    command.h \
    deck.h \
    gamemanager.h \
    gamestate.h \
    mainwindow.h \
    manacollection.h \
    phase.h \
    player.h \
    scenebot.h \
    stackobject.h \
    textparser.h \
    type.h \
    zone.h

FORMS += \
    mainwindow.ui \
    manacollection.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Icons.qrc \
    TextFiles.qrc
