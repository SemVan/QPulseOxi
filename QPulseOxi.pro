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
    keepncalc.cpp \
    cameratool.cpp \
    displayer.cpp \
    imageprocessor.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    protocol.h \
    keepncalc.h \
    cameratool.h \
    displayer.h \
    imageprocessor.h

FORMS    += mainwindow.ui

#INCLUDEPATH += C:/Coding/fftw/

#LIBS +=  C:/Coding/fftw/libfftw3-3.dll
#LIBS +=  C:/Coding/FFTW/v_3.3.6/libfftw3f-3.dll
#LIBS +=  C:/Coding/FFTW/v_3.3.6/libfftw3l-3.dll

    INCLUDEPATH += C:\opencvmingw\install\include\


    LIBS +=C:\opencvmingw\install\x86\mingw\bin\libopencv_core310.dll
    LIBS +=C:\opencvmingw\install\x86\mingw\bin\libopencv_highgui310.dll
    LIBS +=C:\opencvmingw\install\x86\mingw\bin\libopencv_imgcodecs310.dll
    LIBS +=C:\opencvmingw\install\x86\mingw\bin\libopencv_videoio310.dll
    LIBS +=C:\opencvmingw\install\x86\mingw\bin\libopencv_imgproc310.dll
    LIBS +=C:\opencvmingw\install\x86\mingw\bin\libopencv_objdetect310.dll

    LIBS +=C:\opencvmingw\install\x86\mingw\lib\libopencv_core310.dll.a
    LIBS +=C:\opencvmingw\install\x86\mingw\lib\libopencv_highgui310.dll.a
    LIBS +=C:\opencvmingw\install\x86\mingw\lib\libopencv_imgcodecs310.dll.a
    LIBS +=C:\opencvmingw\install\x86\mingw\lib\libopencv_videoio310.dll.a
    LIBS +=C:\opencvmingw\install\x86\mingw\lib\libopencv_imgproc310.dll.a
    LIBS +=C:\opencvmingw\install\x86\mingw\lib\libopencv_objdetect310.dll.a

