PRECOMPILED_HEADER = ../Src/qtincludes.h

INCLUDEPATH += ../ \
    GUI \

SOURCES += \
    Src/main.cpp \
    Src/intellect.cpp \
    GUI/intellectmainwindowex.cpp \
    GUI/inputpassword.cpp \
    ../Src/Memory/elementsmanager.cpp \
    ../Src/Memory/memorymanager.cpp \
    ../Src/Memory/memorywrapper.cpp \
    ../Src/Memory/mewrapper.cpp \
    ../Src/Memory/qmemorymodel.cpp \
    ../Src/Memory/qmemoryselectionmodel.cpp \
    ../Src/Memory/tme.cpp \
    ../Src/Memory/tmemory.cpp \
    ../Src/Memory/tmevalue.cpp \
    ../Src/Memory/memorycompareproxymodel.cpp \
    ../Src/Memory/memoryeditor.cpp \
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
    ../Src/GUI/MainWindowEx/mainwindowex.cpp \
    ../Src/GUI/MemoryView/memorytreeview.cpp \
    ../Src/GUI/MemoryView/memorytableview.cpp \
    ../Src/GUI/MemoryView/memoryeditordelegate.cpp \
    ../Src/GUI/MemoryView/memorylistview.cpp \
    ../Src/GUI/memorycontroller.cpp \
    ../Src/GUI/statusbar.cpp \
    ../Src/GUI/menu.cpp \
    ../Src/GUI/toolbar.cpp \
    ../Src/GUI/logview.cpp \
    ../Src/GUI/graphicsview.cpp \
    ../Src/GUI/filedialog.cpp \
    ../Src/GUI/memoryeditorwgt.cpp \
    ../Src/GUI/ScriptEditor/highlighter.cpp \
    ../Src/GUI/ScriptEditor/scripteditor.cpp \
    ../Third/qmlcreator/cpp/QMLHighlighter.cpp \
    ../Third/qmlcreator/cpp/SyntaxHighlighter.cpp \
    ../Src/QmlManager/qmlmanager.cpp \
    ../Src/Plugins/qmlregistertypes.cpp

HEADERS  += \
    Src/intellect.h \
    GUI/intellectmainwindowex.h \
    GUI/inputpassword.h \
    ../Src/Memory/elementsmanager.h \
    ../Src/Memory/memorymanager.h \
    ../Src/Memory/memorywrapper.h \
    ../Src/Memory/mewrapper.h \
    ../Src/Memory/qmemorymodel.h \
    ../Src/Memory/qmemoryselectionmodel.h \
    ../Src/Memory/tme.h \
    ../Src/Memory/tmemory.h \
    ../Src/Memory/tmevalue.h \
    ../Src/Memory/memorycompareproxymodel.h \
    ../Src/Memory/memoryeditor.h \
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
    ../Src/GUI/MainWindowEx/mainwindowex.h \
    ../Src/GUI/MemoryView/memorytreeview.h \
    ../Src/GUI/MemoryView/memorytableview.h \
    ../Src/GUI/MemoryView/memorylistview.h \
    ../Src/GUI/MemoryView/memoryeditordelegate.h \
    ../Src/GUI/memorycontroller.h \
    ../Src/GUI/statusbar.h \
    ../Src/GUI/menu.h \
    ../Src/GUI/toolbar.h \
    ../Src/GUI/logview.h \
    ../Src/GUI/graphicsview.h \
    ../Src/GUI/filedialog.h \
    ../Src/GUI/memoryeditorwgt.h \
    ../Src/GUI/ScriptEditor/highlighter.h \
    ../Src/GUI/ScriptEditor/scripteditor.h \
    ../Third/qmlcreator/cpp/QMLHighlighter.h \
    ../Third/qmlcreator/cpp/SyntaxHighlighter.h \
    ../Src/QmlManager/qmlmanager.h \
    ../Src/Plugins/qmlregistertypes.h \
    ../Src/qtincludes.h

FORMS    += \
    GUI/dialogmemoryeditor.ui \
    ../Src/GUI/MainWindowEx/mainwindowex.ui

RESOURCES += \
    images.qrc \
    ../resources/dictionaries.qrc
