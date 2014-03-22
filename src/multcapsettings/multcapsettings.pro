QT += core gui widgets multimedia multimediawidgets
TARGET = multicapsettings
TEMPLATE = app
CONFIG += c++11
DESTDIR = $$PWD/../../bin
HEADERS += \
    widget.h

SOURCES += \
    main.cpp\
    widget.cpp

FORMS += \
    widget.ui
