import QtQuick 2.10
import QtQuick.Controls 2.3

Item {
	id: root
	property var memModel
	property var me
	property bool readOnly: false
	//property var memEditor
	property real vPosition: scrollView.ScrollBar.vertical.position
	property real hPosition: scrollView.ScrollBar.horizontal.position

	onMemModelChanged: {
		if(memModel)
			me = memModel.selected
	}

	onMeChanged: {
		showValue()
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

	function setVPosition(_val) {
		if(scrollView.ScrollBar.vertical.position !== _val)
			scrollView.ScrollBar.vertical.position = _val
	}

	function setHPosition(_val) {
		if(scrollView.ScrollBar.horizontal.position !== _val)
			scrollView.ScrollBar.horizontal.position = _val
	}
	Connections {
		target: root.memModel ? root.memModel : null
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

	ScrollView {
		id: scrollView

		anchors.fill: parent
//		contentWidth: edit.contentWidth
//		contentHeight: edit.paintedHeight+20
		clip: true

//		function ensureVisible(r)
//		{
//			if (contentX >= r.x)
//				contentX = r.x;
//			else if (contentX+width <= r.x+r.width)
//				contentX = r.x+r.width-width;
//			if (contentY >= r.y)
//				contentY = r.y;
//			else if (contentY+height <= r.y+r.height)
//				contentY = r.y+r.height-height;
//		}

		TextArea {
			id: edit
			focus: true
			width: scrollView.width
			//wrapMode: TextEdit.Wrap
			//anchors.fill: parent
			placeholderText: qsTr("Input value here")
			selectByMouse: true
			property bool canChange

			background: Rectangle {
				//implicitWidth: 200
				//implicitHeight: 40
				height: Math.max(edit.implicitHeight, edit.height)
				width: Math.max(edit.implicitWidth, edit.width)
				border.color: edit.enabled ? "#21be2b" : "transparent"
			}

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

			//onCursorRectangleChanged: scrollView.ensureVisible(cursorRectangle)
		}
	}
	// Таймер для отложенного сохранения
	Timer {
		id: timer
		interval: 1000
		onTriggered: doSave()
	}
}
