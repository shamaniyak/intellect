import QtQuick 2.10
import QtQuick.Controls 1.4

TreeView {
    id: root
    anchors.fill: parent

    TableViewColumn {
        title: "Name"
        role: "display"
        width: 200
    }
}
