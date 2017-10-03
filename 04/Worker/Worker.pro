#-------------------------------------------------
#
# Project created by QtCreator 2015-11-30T21:34:06
#
#-------------------------------------------------

CONFIG += c++14
QT       += core gui
QT += script scripttools
QT += qml quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Worker
TEMPLATE = app

win32 {
  DESTDIR = ../exe
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

INCLUDEPATH += ../ \
    GUI \
    GUI/MemoryView

SOURCES += \
    GUI/contextmenu.cpp \
    GUI/mainwindowex.cpp \
    GUI/highlighter.cpp \
    GUI/memoryeditor.cpp \
    GUI/MemoryView/memorytreeview.cpp \
    GUI/MemoryView/mytreewidgetitem.cpp \
    GUI/MemoryView/memorytableview.cpp \
    GUI/MemoryView/memorylistview.cpp \
    GUI/logview.cpp \
    GUI/scripteditor.cpp \
    GUI/graphicsview.cpp \
    GUI/toolbar.cpp \
    GUI/filedialog.cpp \
    GUI/MemoryView/memoryeditordelegate.cpp \
    GUI/menu.cpp \
    ../Src/Memory/elementsmanager.cpp \
    ../Src/Memory/memorymanager.cpp \
    ../Src/Memory/memorywrapper.cpp \
    ../Src/Memory/mewrapper.cpp \
    ../Src/Memory/qmemorymodel.cpp \
    ../Src/Memory/qmemoryselectionmodel.cpp \
    ../Src/Memory/tme.cpp \
    ../Src/Memory/tmemory.cpp \
    ../Src/Memory/tmevalue.cpp \
    ../Src/Plugins/intellectplugin.cpp \
    ../Src/Plugins/memoryplugin.cpp \
    ../Src/Plugins/qeventscriptplugin.cpp \
    ../Src/Plugins/qtwidgetsplugin.cpp \
    ../Src/commander.cpp \
    ../Src/iobject.cpp \
    ../Src/modelmanager.cpp \
    ../Src/namemanager.cpp \
    ../Src/objectmanager.cpp \
    ../Src/pluginmanager.cpp \
    ../Src/qqmlwrapper.cpp \
    ../Src/scriptwrapper.cpp \
    ../Src/swmrg.cpp \
    ../Src/talgorithm.cpp \
    ../Src/windowmanager.cpp \
    Src/main.cpp \
    GUI/dialogmemoryeditor.cpp \
    GUI/workermainwindowex.cpp \
    Src/worker.cpp

HEADERS  += \
    GUI/contextmenu.h \
    GUI/mainwindowex.h \
    GUI/highlighter.h \
    GUI/memoryeditor.h \
    GUI/MemoryView/memorytreeview.h \
    GUI/MemoryView/mytreewidgetitem.h \
    GUI/MemoryView/memorytableview.h \
    GUI/MemoryView/memorylistview.h \
    GUI/logview.h \
    GUI/scripteditor.h \
    GUI/graphicsview.h \
    GUI/toolbar.h \
    GUI/filedialog.h \
    GUI/MemoryView/memoryeditordelegate.h \
    GUI/menu.h \
    ../Src/Memory/elementsmanager.h \
    ../Src/Memory/memorymanager.h \
    ../Src/Memory/memorywrapper.h \
    ../Src/Memory/mewrapper.h \
    ../Src/Memory/qmemorymodel.h \
    ../Src/Memory/qmemoryselectionmodel.h \
    ../Src/Memory/tme.h \
    ../Src/Memory/tmemory.h \
    ../Src/Memory/tmevalue.h \
    ../Src/Plugins/intellectplugin.h \
    ../Src/Plugins/memoryplugin.h \
    ../Src/Plugins/qeventscriptplugin.h \
    ../Src/Plugins/qtwidgetsplugin.h \
    ../Src/commander.h \
    ../Src/imemory.h \
    ../Src/iobject.h \
    ../Src/modelmanager.h \
    ../Src/namemanager.h \
    ../Src/objectmanager.h \
    ../Src/pluginmanager.h \
    ../Src/qqmlwrapper.h \
    ../Src/scriptwrapper.h \
    ../Src/swmrg.h \
    ../Src/talgorithm.h \
    ../Src/windowmanager.h \
    GUI/dialogmemoryeditor.h \
    GUI/workermainwindowex.h \
    Src/worker.h

FORMS    += \
    GUI/mainwindowex.ui \
    GUI/dialogmemoryeditor.ui

DISTFILES +=

RESOURCES += \
    images.qrc
