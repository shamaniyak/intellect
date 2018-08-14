import QtQuick 2.10
import QtQuick.Dialogs 1.2

FileDialog {
	id: fileSaveDialog
	selectExisting: false
	title: qsTr("Please enter a file")
	folder: II.applicationDirPath()
	property var memModel
	signal saved()
	onAccepted: {
		//console.log("You chose: " + fileSaveDialog.fileUrl)
		var filePath = II.urlToNativeFilePath(fileSaveDialog.fileUrl)
		console.log("You chose: " + filePath)
		saveCommand(filePath)
		saved()
	}
	onRejected: {
		console.log("Canceled")
	}

	function saveCommand(_path) {
		memModel.filePath = _path
		memModel.save()
	}
}
