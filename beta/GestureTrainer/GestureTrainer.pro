#-------------------------------------------------
#
# Project created by QtCreator 2013-04-27T14:01:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GestureTrainer
TEMPLATE = app


SOURCES += main.cpp\
    forms/mainwindow.cpp \
    ext/skindetectcontroller.cpp \
    ext/skindetector.cpp

HEADERS  += forms/mainwindow.h \
    ext/skindetector.h \
    ext/skindetectcontroller.h

FORMS    +=  forms/mainwindow.ui

INCLUDEPATH += /usr/local/include/
LIBS += -L/usr/local/lib/ \
   -lopencv_core \
   -lopencv_highgui \
   -lopencv_imgproc \
   -lopencv_features2d \
   -lopencv_objdetect \
   -lopencv_calib3d
