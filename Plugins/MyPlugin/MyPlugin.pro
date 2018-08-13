#-------------------------------------------------
#
# Project created by QtCreator 2017-05-16T00:27:04
#
#-------------------------------------------------

QT  += core
QT       -= gui

TARGET = MyPlugin
TEMPLATE = lib
CONFIG += c++14 plugin

win32 {
  DESTDIR = ../../exe/plugins
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

DEFINES += MYPLUGIN_LIBRARY

INCLUDEPATH += \
  "../.."

SOURCES += myplugin.cpp

HEADERS += myplugin.h\
        myplugin_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
