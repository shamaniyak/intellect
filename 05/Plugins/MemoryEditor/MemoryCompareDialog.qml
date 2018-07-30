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

	function findPos(findFirst) {
		var pos
		if(findFirst)
			pos = textDiff.findFirst()
		else
			pos = textDiff.findPos()
		var editor = editSrc.editor.text.length > editCompare.editor.text.length ? editSrc.editor : editCompare.editor
		editor.select(pos.pos, pos.pos+pos.len)
		editor.selectionColor = "red"
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
				textDiff.diff2Texts(editCompare.editor.text, editSrc.editor.text)
				findPos(true)
				//console.debug("MemoryCompareDialog.qml:97",pos.pos, pos.len)
				//var pos = findPos(editSrc.editor.text, editCompare.editor.text)
				//console.debug("MemoryCompareDialog.qml:94",pos)
				//if(pos >= 0)
					//editSrc.editor.cursorPosition = pos
				//editSrc.editor.selectedTextColor = "red"
				//editSrc.editor.selectAll()
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

	Diff {
		id: textDiff
		property int posStart: -1
		property int posEnd: -1

		function diff2Texts(_a, _b) {
			diff(_a, _b)
			console.log("editdistance:" + editdistance());
			//console.log("lcs:" + getlcs());
			console.log("ses");

			var i   = 0;
			var ses = getses();
			var ses_t = ses.length ? ses[0].t : ses_COMMON
			var ses_elem = ses.length ? ses[0].elem : ""
			for (i=0;i<ses.length;++i) {
				if(ses[i].t === ses_t)
					ses_elem += ses[i].elem
				else {
					var SES = ["-", "", "+"]
					console.log(SES[ses_t+1], ses_elem)
					ses_t = ses[i].t
					ses_elem = ses[i].elem
				}
			}
		}

		function findFirst() {
			posStart = -1
			return findPos()
		}

		function findPos() {
			console.debug(posStart)
			var ses = getses();
			var i   = posStart < 0 ? 0 : posStart;
			var len = 0;
			var ses_t = i < ses.length ? ses[i].t : ses_COMMON
			for (;i<ses.length;++i) {
				if(ses[i].t === ses_t) {
					len++
					continue
				}
				else {
					if(ses_t === ses_ADD) {
						posStart = i
						return {"pos": i-len, "len": len}
					}
					ses_t = ses[i].t
					len = 1
				}
			}
			posStart = -1
			return {"pos": 0, "len": 0}
		}
	}
}
