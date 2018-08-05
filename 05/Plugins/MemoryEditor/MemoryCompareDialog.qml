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

	Diff {
		id: textDiff
		property int posStart: -1
		property int posEnd: -1
		property int lenSum: 0
		property int lenSumAdd: 0
		property bool canPrintLog: false

		function diff2Texts(_a, _b) {
			diff(_a, _b)

			if(canPrintLog)
				printLog()
		}

		function printLog() {
			console.log("editdistance:" + editdistance());
			//console.log("lcs:" + getlcs());
			console.log("ses");

			var SES = ["-", "", "+"]
			var i   = 0;
			var ses = getses();
			var ses_t = ses.length ? ses[0].t : ses_COMMON
			var ses_elem = ""
			for (i=0;i<ses.length;++i) {
				if(ses[i].t === ses_t)
					ses_elem += ses[i].elem
				else {
					console.log(SES[ses_t+1], ses_elem)
					ses_t = ses[i].t
					ses_elem = ses[i].elem
				}
			}
			console.log(SES[ses_t+1], ses_elem)
		}

		function findFirst() {
			posStart = -1
			lenSum = 0
			lenSumAdd = 0
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
					posStart = i
					if(ses_t === ses_ADD || ses_t === ses_DELETE) {
						return makePos(ses_t, i, len)
					}

					ses_t = ses[i].t
					len = 1
				}
			}
			var result = {"pos": 0, "len": 0, "t": 0}
			if(ses_t === ses_ADD || ses_t === ses_DELETE) {
				result = makePos(ses_t, i, len)
			}
			posStart = -1
			lenSum = 0
			lenSumAdd = 0
			return result
		}

		function makePos(t, p, l) {
			if(t === ses_ADD) {
				lenSumAdd += l
				return {"pos": p-l-lenSum, "len": l, "t": t}
			}
			if(t === ses_DELETE) {
				lenSum += l
				return {"pos": p-l-lenSumAdd, "len": l, "t": t}
			}
			return {"pos": 0, "len": 0, "t": 0}
		}
	}
}
