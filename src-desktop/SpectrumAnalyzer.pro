#-------------------------------------------------
#
# Project created by QtCreator 2018-03-23T09:58:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11 static

TARGET = SpectrumAnalyzer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# serial library
DEPENDPATH += ../lib/serial
INCLUDEPATH += ../lib/serial/include

linux:LIBS += -L../lib/build-serial
win32:LIBS += -L../lib/build-serial/debug

LIBS += -lserial

# statically link libraries on windows
win32:LIBS += -static -static-libgcc -static-libstdc++


# qcustomplot library
INCLUDEPATH += ../lib/qcustomplot

# sources
SOURCES += \
    ../lib/qcustomplot/qcustomplot.cpp \
    main.cpp \
    mainwindow.cpp \
    serialworker.cpp

HEADERS += \
    ../lib/qcustomplot/qcustomplot.h \
    mainwindow.h \
    serialworker.h

FORMS += \
        mainwindow.ui
