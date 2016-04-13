#-------------------------------------------------
#
# Project created by QtCreator 2016-03-04T14:55:11
#
#-------------------------------------------------

QT       +=  core widgets serialport

TARGET = interface
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    serialwaiterdialog.cpp

HEADERS  += widget.h \
    serialwaiterdialog.h

FORMS    += widget.ui

