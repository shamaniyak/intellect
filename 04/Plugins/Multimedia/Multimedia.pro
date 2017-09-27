#-------------------------------------------------
#
# Project created by QtCreator 2017-07-05T00:34:23
#
#-------------------------------------------------

QT       += widgets
QT += multimedia multimediawidgets

QT       -= gui

TARGET = Multimedia
TEMPLATE = lib
CONFIG += qt warn_on plugin c++14

DEFINES += MULTIMEDIA_LIBRARY

INCLUDEPATH += \
  ../../Interfaces

SOURCES += multimedia.cpp

HEADERS += multimedia.h\
        multimedia_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    DESTDIR = ../../Exe/Plugins
    UI_DIR = .ui/win
    MOC_DIR = .moc/win
    OBJECTS_DIR = .obj/win
}
