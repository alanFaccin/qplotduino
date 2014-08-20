#-------------------------------------------------
#
# Project created by QtCreator 2014-07-21T22:22:49
#
#-------------------------------------------------

QT       += core gui widgets printsupport serialport

TARGET = qPlot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    channel.cpp \
    serialcommunication.cpp \
    rxchannel.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    channel.h \
    serialcommunication.h \
    rxchannel.h

FORMS    += mainwindow.ui \
    rxchannel.ui
