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

//	header: ToolBar {
//		height: menuBar.height
//		Material.background: Theme.primaryColor()
//		MainMenu {
//			id: menuBar
//		}
//	}

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

	function compare() {
		compareModel.compare()
	}

	Row {
		width: parent.width
		height: parent.height

		MemoryTreeView {
			id: dstMemoryTreeView
			width: parent.width / 2
			height: parent.height
			model: compareModel

			// Контекстное меню для дерева
			treeMenu: Menu {
				MenuItem {
					text: qsTr("Open source")
					implicitHeight: 25
					onTriggered: {
						openDlg.memModel = srcMem
						openDlg.open()
					}
				}
				MenuItem {
					text: qsTr("Open dest")
					implicitHeight: 25
					onTriggered: {
						openDlg.memModel = compareMem
						//openDlg.accepted.connect(onAccepted)
						openDlg.open()
					}

					function onAccepted() {
						openDlg.accepted.disconnect(onAccepted)
						compareModel.compare()
					}
				}
			}
		}
	}

	OpenMemoryDialog {
		id: openDlg

		onAccepted: {
			compare()
		}
	}
}
