QT += widgets network sql

CONFIG += c++11

TARGET = player1
TEMPLATE = app

SOURCES += \
    main.cpp \
    cryptomanager.cpp \
    gamelogic.cpp \
    mainwindow.cpp \
    networkmanager.cpp

HEADERS += \
    mainwindow.h \
    cryptomanager.h \
    gamelogic.h \
    networkmanager.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += /usr/include/openssl
LIBS += -lssl -lcrypto
