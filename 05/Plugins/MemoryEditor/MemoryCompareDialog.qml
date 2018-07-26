import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.3
import MemoryManager 1.0

ApplicationWindow {
	id: mainWindow
	visible: true
	width: 640
	height: 480
	title: qsTr("Memory Compare")

	MemoryModel {
		id: srcMem
	}

	MemoryModel {
		id: compareMem
	}

	MemoryCompareProxyModel {
		id: compareModel
		sourceMemory: srcMem
		compareMemory: compareMem
	}

	Column {
		anchors.fill: parent

		Row {
			id: headerRow
			width: parent.width
			height: 32
			Rectangle {
				height: parent.height
				width: parent.width / 2
				color: "steelblue"
				Text {
					anchors.fill: parent
					font.pixelSize: 16
					text: qsTr("Source")
				}
			}
			Rectangle {
				height: parent.height
				width: parent.width / 2
				color: "steelblue"
				Text {
					anchors.fill: parent
					font.pixelSize: 16
					text: qsTr("Dest")
				}
				Button {
					anchors.top: parent.top
					anchors.right: parent.right
					width: 40
					height: 25
					text: qsTr("Refresh")
					onClicked: {
						compareModel.compare()
					}
				}
			}
		}

		Row {
			width: parent.width
			height: parent.height - headerRow.height

			MemoryTreeView {
				id: srcMemoryTreeView
				width: parent.width / 2
				height: parent.height
				model: srcMem

				// Контекстное меню для дерева
				treeMenu: Menu {
					MenuItem {
						text: qsTr("Open")
						implicitHeight: 25
						onTriggered: {
							openDlg.memModel = srcMem
							openDlg.open()
						}
					}
				}
			}

			MemoryTreeView {
				id: dstMemoryTreeView
				width: parent.width / 2
				height: parent.height
				model: compareModel

				// Контекстное меню для дерева
				treeMenu: Menu {
					MenuItem {
						text: qsTr("Open")
						implicitHeight: 25
						onTriggered: {
							openDlg.memModel = compareMem
							openDlg.open()
						}
					}
				}
			}
		}
	}

	OpenMemoryDialog {
		id: openDlg
	}

	function compare() {
		//
	}
}
