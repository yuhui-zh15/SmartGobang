#-------------------------------------------------
#
# Project created by QtCreator 2016-08-30T15:28:36
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmartGobang
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    chessboard.cpp \
    chesslabel.cpp \
    createdialog.cpp \
    connectdialog.cpp

HEADERS  += mainwindow.h \
    chessboard.h \
    chesslabel.h \
    createdialog.h \
    connectdialog.h

FORMS    += \
    createdialog.ui \
    connectdialog.ui

RESOURCES += \
    img.qrc
