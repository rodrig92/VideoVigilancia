#-------------------------------------------------
#
# Project created by QtCreator 2014-04-15T19:56:33
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia multimediawidgets
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cliente
TEMPLATE = app


SOURCES += main.cpp\
        cliente.cpp \
    frames.cpp \
    deteccionfondo.cpp

HEADERS  += cliente.h \
    frames.h \
    deteccionfondo.h

FORMS    += cliente.ui

include(QtOpenCV.pri)
add_opencv_modules(core video imgproc)
#add_opencv_modules(core video imgproc, 2.4.4)

#unix: CONFIG += link_pkgconfig
#unix: PKGCONFIG += opencv
