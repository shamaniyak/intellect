#-------------------------------------------------
#
# Project created by QtCreator 2017-05-27T01:31:04
#
#-------------------------------------------------

QT  += core
QT       -= gui

TARGET = SppzSituationToQt
TEMPLATE = lib
CONFIG += c++14 plugin

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

DEFINES += SPPZSITUATIONTOQT_LIBRARY

INCLUDEPATH += \
  "../.." \
  "d:/Work/sppz/Include/Classifier" \
  "d:/Work/sppz/Include/situation"

SOURCES += sppzsituationtoqt.cpp

HEADERS += sppzsituationtoqt.h\
        sppzsituationtoqt_global.h \
    ../../Interfaces/isppzsituationplugin.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
