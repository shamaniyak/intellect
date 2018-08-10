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

	property var sourceMemory: srcMem

	MemoryModel {
		id: srcMem
	}

	MemoryModel {
		id: compareMem
	}

	MemoryCompareProxyModel {
		id: compareModel
		sourceMemory: mainWindow.sourceMemory
		compareMemory: compareMem
	}

	function compare() {
		compareModel.compare()
	}

	function selectElement(index) {
		var me = compareModel.getMeByIndex(index)
		var me1 = sourceMemory.get(me.path)
		var me2 = compareMem.get(me.path)
		editSrc.me = me1
		editCompare.me = me2
		textDiff.diff2Texts(editCompare.editor.text, editSrc.editor.text)
		findPos(true)
	}

	function findPos(findFirst) {
		var pos
		if(findFirst)
			pos = textDiff.findFirst()
		else
			pos = textDiff.findPos()
		var color = "green"
		if(pos.t === 1) {
			editCompare.editor.deselect()
		}
		else {
			editSrc.editor.deselect()
			color = "red"
		}

		var editor = pos.t === 1 ? editSrc.editor : editCompare.editor
		editor.select(pos.pos, pos.pos+pos.len)
		editor.selectionColor = color
	}

	header: ToolBar {
		height: 32

		Row {
			anchors.fill: parent
			ToolButton {
				height: parent.height
				text: qsTr("Find")
				onClicked: findPos(false)
			}
		}
	}

	Row {
		id: contentView
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
						sourceMemory = srcMem
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
				selectElement(index)
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

	DiffEx {
		id: textDiff
	}
}
