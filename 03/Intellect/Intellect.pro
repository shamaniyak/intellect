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

TARGET = Intellect
TEMPLATE = app

win32 {
  DESTDIR = ../exe
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

INCLUDEPATH += ../ \
    Src/ \
    Src/Memory \
    GUI \
    GUI/MemoryView

SOURCES += Src/main.cpp \
    Src/scriptwrapper.cpp \
    Src/intellect.cpp \
    GUI/dialogmemoryeditor.cpp \
    Src/talgorithm.cpp \
    Src/Memory/elementsmanager.cpp \
    Src/Memory/mewrapper.cpp \
    Src/Memory/tmevalue.cpp \
    Src/Plugins/qtwidgetsplugin.cpp \
    Src/Memory/memorywrapper.cpp \
    Src/Memory/tmemory.cpp \
    Src/Memory/tme.cpp \
    Src/iobject.cpp \
    Src/objectmanager.cpp \
    Src/Plugins/memoryplugin.cpp \
    GUI/contextmenu.cpp \
    GUI/mainwindowex.cpp \
    Src/windowmanager.cpp \
    Src/Plugins/intellectplugin.cpp \
    GUI/highlighter.cpp \
    Src/qqmlwrapper.cpp \
    GUI/memoryeditor.cpp \
    GUI/MemoryView/memorytreeview.cpp \
    GUI/MemoryView/mytreewidgetitem.cpp \
    Src/modelmanager.cpp \
    GUI/MemoryView/memorytableview.cpp \
    GUI/MemoryView/memorylistview.cpp \
    Src/Memory/qmemorymodel.cpp \
    Src/namemanager.cpp \
    Src/commander.cpp \
    Src/Plugins/qeventscriptplugin.cpp \
    Src/pluginmanager.cpp \
    Src/Memory/qmemoryselectionmodel.cpp \
    GUI/logview.cpp \
    GUI/scripteditor.cpp \
    GUI/graphicsview.cpp \
    GUI/toolbar.cpp \
    GUI/filedialog.cpp \
    GUI/MemoryView/memoryeditordelegate.cpp \
    Test/TestSppzCartography/testsppzcartography.cpp \
    Src/Memory/memorymanager.cpp \
    GUI/menu.cpp \
    GUI/intellectmainwindowex.cpp \
    Src/worker.cpp

HEADERS  += \
    Src/scriptwrapper.h \
    Src/intellect.h \
    GUI/dialogmemoryeditor.h \
    Src/talgorithm.h \
    Src/Memory/elementsmanager.h \
    Src/Memory/mewrapper.h \
    Src/Memory/tmevalue.h \
    Src/Plugins/qtwidgetsplugin.h \
    Src/Memory/memorywrapper.h \
    Src/Memory/tmemory.h \
    Src/Memory/tme.h \
    ../Interfaces/imemory.h \
    ../Interfaces/imemoryitem.h \
    Src/iobject.h \
    Src/objectmanager.h \
    Src/Plugins/memoryplugin.h \
    GUI/contextmenu.h \
    GUI/mainwindowex.h \
    Src/windowmanager.h \
    Src/Plugins/intellectplugin.h \
    GUI/highlighter.h \
    Src/qqmlwrapper.h \
    GUI/memoryeditor.h \
    GUI/MemoryView/memorytreeview.h \
    GUI/MemoryView/mytreewidgetitem.h \
    Src/modelmanager.h \
    GUI/MemoryView/memorytableview.h \
    GUI/MemoryView/memorylistview.h \
    Src/Memory/qmemorymodel.h \
    ../Interfaces/iparent.h \
    ../Interfaces/iplugin.h \
    Src/namemanager.h \
    Src/commander.h \
    Src/Plugins/qeventscriptplugin.h \
    Src/pluginmanager.h \
    Src/Memory/qmemoryselectionmodel.h \
    GUI/logview.h \
    GUI/scripteditor.h \
    GUI/graphicsview.h \
    GUI/toolbar.h \
    GUI/filedialog.h \
    GUI/MemoryView/memoryeditordelegate.h \
    Test/TestSppzCartography/testsppzcartography.h \
    Src/Memory/memorymanager.h \
    GUI/menu.h \
    GUI/intellectmainwindowex.h \
    Src/worker.h

FORMS    += \
    GUI/dialogmemoryeditor.ui \
    GUI/mainwindowex.ui

DISTFILES +=

RESOURCES += \
    images.qrc
