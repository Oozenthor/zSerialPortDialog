#-------------------------------------------------
#
# Project created by QtCreator 2014-11-14T11:01:04
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zSerialPortDialog
TEMPLATE = app


SOURCES += main.cpp\
    zserialdialog.cpp \
    serialexample.cpp

HEADERS  += \
    zserialdialog.h \
    serialexample.h

FORMS    += \
    zserialdialog.ui \
    serialexample.ui

OTHER_FILES += \
    LICENSE

RESOURCES += \
    icons.qrc
