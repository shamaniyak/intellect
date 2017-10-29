#-------------------------------------------------
#
# Project created by QtCreator 2017-06-08T16:17:10
#
#-------------------------------------------------

QT += winextras widgets

TARGET = Cartography
TEMPLATE = lib
CONFIG += qt warn_on plugin c++14

DEFINES += CARTOGRAPHY_LIBRARY _USE_MATH_DEFINES

INCLUDEPATH += \
  MapView \
  MapView/Include \
  ../../Interfaces \
  "d:/work/sppz/Include"

SOURCES += sppzcartography.cpp \
    MapView/MapView.cpp \
    cartography.cpp \
    cartographymap.cpp \
    MapView/mapgraphicspixmapitem.cpp

HEADERS += sppzcartography.h\
        sppzcartography_global.h \
    MapView/MapView.h \
    cartography.h \
    cartographymap.h \
    MapView/mapgraphicspixmapitem.h

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
