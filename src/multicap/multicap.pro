QT += core network
QT -= gui
CONFIG += c++11
DESTDIR = $$PWD/../../bin
TARGET = multicap
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += \
    main.cpp
