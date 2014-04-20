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
    frames.cpp

HEADERS  += cliente.h \
    frames.h

FORMS    += cliente.ui
