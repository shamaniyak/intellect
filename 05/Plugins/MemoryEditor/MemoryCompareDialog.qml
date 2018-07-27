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
			property int minWidth: 200
			property int maxWidth: 350
			width: {
				var w = parent.width / 3;
				if(w < minWidth) w = minWidth
				if(w > maxWidth) w = maxWidth
				return w
			}
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
						openDlg.open()
					}
				}
			}

			onClicked: {
				var me = compareModel.getMeByIndex(index)
				var me1 = srcMem.get(me.path)
				var me2 = compareMem.get(me.path)
				editSrc.me = me1
				editCompare.me = me2
				//editSrc.text = me1.isValid() ? me1.val : ""
				//editCompare.text = me2.isValid() ? me2.val : ""
			}
		}

		Row {
			width: parent.width - dstMemoryTreeView.width
			height: parent.height

			MemoryElementEditor {
				id: editSrc
				width: parent.width / 2
				height: parent.height
				onVPositionChanged: {
					editCompare.setVPosition(vPosition)
				}
				onHPositionChanged: {
					editCompare.setHPosition(hPosition)
				}
			}
			MemoryElementEditor {
				id: editCompare
				width: parent.width / 2
				height: parent.height
				onVPositionChanged: {
					editSrc.setVPosition(vPosition)
				}
				onHPositionChanged: {
					editSrc.setHPosition(hPosition)
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
