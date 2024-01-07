QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    animationwindow.cpp \
    bgmcontrol.cpp \
    buttongroup.cpp \
    card.cpp \
    cardpanel.cpp \
    cards.cpp \
    client.cpp \
    countdown.cpp \
    endingpanel.cpp \
    gamecontrol.cpp \
    gamepanel.cpp \
    loading.cpp \
    player.cpp \
    playhand.cpp \
    robot.cpp \
    robotgraplord.cpp \
    robotplayhand.cpp \
    scorepanel.cpp \
    startbutton.cpp \
    strategy.cpp \
    userplayer.cpp

HEADERS += \
    animationwindow.h \
    bgmcontrol.h \
    buttongroup.h \
    card.h \
    cardpanel.h \
    cards.h \
    countdown.h \
    endingpanel.h \
    gamecontrol.h \
    gamepanel.h \
    loading.h \
    player.h \
    playhand.h \
    robot.h \
    robotgraplord.h \
    robotplayhand.h \
    scorepanel.h \
    startbutton.h \
    strategy.h \
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
