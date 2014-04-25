#-------------------------------------------------
#
# Project created by QtCreator 2014-04-18T13:35:40
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia multimediawidgets
QT       += network
QT       += widgets
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Servidor
TEMPLATE = app


SOURCES += main.cpp\
        servidor.cpp

HEADERS  += servidor.h \
    SqlLite.h

FORMS    += servidor.ui
