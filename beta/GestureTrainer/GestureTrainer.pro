#-------------------------------------------------
#
# Project created by QtCreator 2013-04-27T14:01:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS = -fpermissive -std=c++11

TARGET = GestureTrainer
TEMPLATE = app


SOURCES += main.cpp\
    forms/mainwindow.cpp \
    detectors/skindetector.cpp \
    detectors/skindetectcontroller.cpp \
    detectors/handdetectcontroller.cpp \
    detectors/handdetector.cpp

HEADERS  += forms/mainwindow.h \
    include/colorhistogram.h \
    detectors/skindetector.h \
    detectors/skindetectcontroller.h \
    detectors/handdetectcontroller.h \
    detectors/handdetector.h \
    include/hand.h

FORMS    +=  forms/mainwindow.ui

INCLUDEPATH += /opt/local/include/
LIBS += -L/opt/local/lib/ \
   -lopencv_core \
   -lopencv_highgui \
   -lopencv_imgproc \
   -lopencv_features2d \
   -lopencv_objdetect \
   -lopencv_calib3d \
