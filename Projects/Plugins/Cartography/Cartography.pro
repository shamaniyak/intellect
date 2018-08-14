#-------------------------------------------------
#
# Project created by QtCreator 2017-06-08T16:17:10
#
#-------------------------------------------------

QT += winextras widgets

TARGET = CartographyMap
TEMPLATE = lib
CONFIG += qt warn_on plugin c++14

DEFINES += CARTOGRAPHY_LIBRARY _USE_MATH_DEFINES

INCLUDEPATH += \
  Src/MapView \
  Src/MapView/Include \
  ../../Interfaces \
  "d:/work/sppz/Include"

SOURCES += \
    Src/MapView/MapView.cpp \
    Src/cartography.cpp \
    Src/cartographymap.cpp \
    Src/MapView/mapgraphicspixmapitem.cpp

HEADERS +=\
    Src/MapView/MapView.h \
    Src/cartography.h \
    Src/cartographymap.h \
    Src/MapView/mapgraphicspixmapitem.h

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
