import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.3
import MemoryManager 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Memory Editor")
    property string appDirPath

    Component.onCompleted: {
        //console.log()
        appDirPath = app.applicationDirPath()
        memModel.open(appDirPath + "/memory.moi")
    }

    onClosing: {
        if(memModel.changed()) {
            close.accepted = false
            messageDialog.open()
        }
    }

    // memModel
    MemoryModel {
        id: memModel
    }

    // treeView
    MemoryTreeView {
        id: treeView
        anchors.fill: parent
        model: memModel
    }

    MessageDialog {
          id: messageDialog
          text: "The memory has been modified."
          informativeText: "Do you want to save your changes?"
          standardButtons: MessageDialog.Save | MessageDialog.Discard | MessageDialog.Cancel
          onAccepted: {
              if(memModel.save())
                  console.log("Memory saved.")
              Qt.quit()
          }
          onDiscard: {
              Qt.quit()
          }
      }

    // mouseArea
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onPressed: {
            if(mouse.button == Qt.RightButton)
                treeMenu.popup()
            mouse.accepted = false
        }
    }

    // treeMenu
    Menu {
        id: treeMenu

        MenuItem {
            id: menuItemAdd
            action: addAction
            implicitHeight: 25
        }

        MenuItem {
            id: menuItemDel
            action: removeAction
            implicitHeight: 25
        }

        MenuItem {
            action: clearAction
            implicitHeight: 25
        }
    }

    // addAction
    Action {
        id: addAction
        text: qsTr("&Add")
        onTriggered: {
            var me = memModel.getMeByIndex(treeView.currentIndex)
            memModel.add(me, "new")
        }
    }
    // removeAction
    Action {
        id: removeAction
        text: qsTr("&Del")
        onTriggered: {
            var me = memModel.getMeByIndex(treeView.currentIndex)
            memModel.deleteMe(me)
        }
    }
    // clearAction
    Action {
        id: clearAction
        text: qsTr("&Clear")
        onTriggered: {
            memModel.clear()
        }
    }
}
