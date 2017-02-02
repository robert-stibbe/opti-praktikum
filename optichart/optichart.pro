TEMPLATE = app

CONFIG += c++14

QT += charts
QT += core gui widgets

TARGET = optichart
SOURCES += main.cpp polarchart.cpp \
    path_reduction.cpp
HEADERS += polarchart.h \
    path_reduction.h
