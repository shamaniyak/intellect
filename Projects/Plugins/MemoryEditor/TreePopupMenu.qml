import QtQuick 2.10
import QtQuick.Controls 2.3

// Контекстное меню для дерева
Menu {
    id: treeMenu
    // Ссылка на MemoryEditorActions
    property var actions

    MenuItem {
        id: menuItemAdd
        action: actions.addAction
        implicitHeight: 25
    }

    MenuItem {
        id: menuItemDel
        action: actions.removeAction
        implicitHeight: 25
    }

    MenuItem {
        action: actions.clearAction
        implicitHeight: 25
    }
    MenuItem {
        action: actions.undoAction
        implicitHeight: 25
    }
    MenuItem {
        action: actions.redoAction
        implicitHeight: 25
    }
}

