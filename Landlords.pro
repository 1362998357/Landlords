QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    buttongroup.cpp \
    card.cpp \
    cardpanel.cpp \
    cards.cpp \
    client.cpp \
    gamecontrol.cpp \
    gamepanel.cpp \
    player.cpp \
    robot.cpp \
    scorepanel.cpp \
    startbutton.cpp \
    userplayer.cpp

HEADERS += \
    buttongroup.h \
    card.h \
    cardpanel.h \
    cards.h \
    gamecontrol.h \
    gamepanel.h \
    player.h \
    robot.h \
    scorepanel.h \
    startbutton.h \
    userplayer.h

FORMS += \
    buttongroup.ui \
    gamepanel.ui \
    scorepanel.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
