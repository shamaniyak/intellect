#-------------------------------------------------
#
# Project created by QtCreator 2017-05-16T01:37:33
#
#-------------------------------------------------

QT  += core
QT       -= gui

TARGET = SppzClassifierToQt
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

DEFINES += SPPZCLASSIFIERTOQT_LIBRARY

INCLUDEPATH += \
  "../.." \
  "../../Interfaces/classifier" \
  "d:/Work/sppz/Include/Classifier"

SOURCES += sppzclassifiertoqt.cpp

HEADERS += sppzclassifiertoqt.h\
        sppzclassifiertoqt_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
