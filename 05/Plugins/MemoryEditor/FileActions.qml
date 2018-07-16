import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.2
import MemoryManager 1.0

Item {
	id: root
	property var memModel
	// actionNew
	property Action actionNew: Action {
		id: actionNew
		text: qsTr("New")
		onTriggered: {
			if(memModel.changed()) {
				messageDialog.command = createNew
				messageDialog.open()
			}
			else {
				createNew()
			}
		}
	}

	// actionOpen
	property Action actionOpen: Action {
		id: actionOpen
		text: qsTr("Open")
		onTriggered: {
			if(memModel.changed()) {
				messageDialog.command = showOpenDialog
				messageDialog.open()
			}
			else {
				showOpenDialog()
			}
		}

		function showOpenDialog() {
			fileDialog.open()
		}
	}

	// actionSave
	property Action actionSave: Action {
		id: actionSave
		text: qsTr("Save")
		onTriggered: {
			if(memModel.filePath === "") {
				fileSaveDialog.open()
			}
			else
				memModel.save()
		}
	}

	// actionSaveAs
	property Action actionSaveAs: Action {
		id: actionSaveAs
		text: qsTr("Save As")
		onTriggered: {
			fileSaveDialog.open()
		}
	}

	// actionQuit
	property Action actionQuit: Action {
		id: actionQuit
		text: qsTr("Quit")
		onTriggered: {
			if(memModel.changed()) {
				messageDialog.command = quitCommand
				messageDialog.open()
			}
			else
				quitCommand()
		}

		function quitCommand() {
			Qt.quit()
		}
	}

	// Garbage collector action
	property Action actionGC: Action {
		text: qsTr("GC")
		onTriggered: {
			gc()
		}
	}

	// Test action
	property Action actionTest: Action {
		text: qsTr("Test")
		onTriggered: {
			var me = memModel.me
			var me1 = memModel.selected
			console.debug("!me: ", me.isNull())
			console.debug("me==me1: ", me.uid==me1.uid)
		}
	}

	OpenMemoryDialog {
		id: fileDialog
		memModel: root.memModel
	}

	SaveMemoryDialog {
		id: fileSaveDialog
		memModel: root.memModel
	}

	// Диалог запроса на сохранение
	SaveMemoryMessageDialog {
		id: messageDialog

		onAccepted: {
			doSave()
		}
		onDiscard: {
			doCommand()
		}

		function doSave() {
			if(memModel.filePath === "") {
				fileSaveDialog.saved.connect(saved)
				fileSaveDialog.open()
			}
			else {
				if(memModel.save()) {
					console.log("Memory saved.", memModel.filePath)
					doCommand()
				}
			}
		}

		function saved() {
			fileSaveDialog.saved.disconnect(saved)
			doCommand()
		}
	}

	function createNew() {
		memModel.filePath = ""
		memModel.clear()
	}
}
