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
  debug {
    DESTDIR = ../../win32/debug/exe/plugins
    UI_DIR = .ui/win32/debug
    MOC_DIR = .moc/win32/debug
    OBJECTS_DIR = .obj/win32/debug
  }
  release {
    DESTDIR = ../../win32/release/exe/plugins
    UI_DIR = .ui/win32/release
    MOC_DIR = .moc/win32/release
    OBJECTS_DIR = .obj/win32/release
  }
}
