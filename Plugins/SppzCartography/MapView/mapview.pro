QT +=  widgets

TEMPLATE = lib

CONFIG += qt warn_on plugin c++14

DEFINES	+= _LARGEFILE64_SOURCE _USE_MATH_DEFINES

INCLUDEPATH += \
  Include \
  ../../../Interfaces

TARGET = mapview

unix {
UI_DIR=.ui/msvs
MOC_DIR=.moc/msvs
OBJECTS_DIR=.obj/msvs
}
win32 {
UI_DIR=.ui/win
MOC_DIR=.moc/win
OBJECTS_DIR=.obj/win
}

DESTDIR=../../../Exe/Plugins

DEFINES += MAPVIEW_LIBRARY

SOURCES	+= MapView.cpp

HEADERS	+= MapView.h
