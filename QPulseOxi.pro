#-------------------------------------------------
#
# Project created by QtCreator 2017-01-24T12:40:22
#
#-------------------------------------------------

QT       += core gui serialport printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QPulseOxi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    protocol.cpp \
    keepncalc.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    protocol.h \
    keepncalc.h

FORMS    += mainwindow.ui

INCLUDEPATH += C:/Coding/fftw/

LIBS +=  C:/Coding/fftw/libfftw3-3.dll
#LIBS +=  C:/Coding/FFTW/v_3.3.6/libfftw3f-3.dll
#LIBS +=  C:/Coding/FFTW/v_3.3.6/libfftw3l-3.dll
