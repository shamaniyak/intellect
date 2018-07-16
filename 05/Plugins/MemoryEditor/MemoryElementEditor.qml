import QtQuick 2.10
import QtQuick.Controls 2.3

Item {
	id: root
	property var memModel
	property var me
	//property var memEditor

	onMemModelChanged: {
		if(memModel)
			me = memModel.selected
	}

//	onSelectedChanged: {
//		showSelectedValue()
//	}

	function showValue() {
		//console.debug("selected = ", selected)
		if(!me.isNull() && me.val !== undefined)
			edit.showText(me.val)
		else
			edit.showText("")
	}

	Connections {
		target: root.memModel
		onValueChanged: {
			if(me.uid === root.me.uid)
				showValue()
		}
		onSelectedChanged: {
			showValue()
		}
	}

	function doSave() {
		//console.debug("doSave")
		//console.trace()
		if(!me.isNull()) {
			if(memEditor)
				memEditor.setVal(me, edit.text)
			else
				me.val = edit.text
		}
	}

	Flickable {
		id: flick

		anchors.fill: parent
		contentWidth: edit.contentWidth
		contentHeight: edit.paintedHeight+20
		clip: true

		function ensureVisible(r)
		{
			if (contentX >= r.x)
				contentX = r.x;
			else if (contentX+width <= r.x+r.width)
				contentX = r.x+r.width-width;
			if (contentY >= r.y)
				contentY = r.y;
			else if (contentY+height <= r.y+r.height)
				contentY = r.y+r.height-height;
		}

		TextArea {
			id: edit
			focus: true
			width: flick.width
			//wrapMode: TextEdit.Wrap
			//anchors.fill: parent
			placeholderText: qsTr("Input value here")
			property bool canChange

			function showText(_text) {
				canChange = false
				text = _text
				canChange = true
			}

			onTextChanged: {
				//console.debug("textChanged")
				if(canChange) {
					timer.stop()
					timer.start()
				}
			}

			onEditingFinished: {
				//console.debug("onEditingFinished")
				if(timer.running) {
					timer.stop()
					doSave()
				}
			}

			onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
		}
	}
	// Таймер для отложенного сохранения
	Timer {
		id: timer
		interval: 1000
		onTriggered: doSave()
	}
}
