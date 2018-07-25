import QtQuick 2.10
import QtQuick.Dialogs 1.2

FileDialog {
	id: fileDialog
	title: qsTr("Please choose a file")
	folder: II.applicationDirPath()
	property var memModel
	onAccepted: {
		//console.log("You chose: " + fileDialog.fileUrls)
		var filePath = II.urlToNativeFilePath(fileDialog.fileUrl)
		console.log("You chose: " + filePath)
		memModel.open(filePath)
	}
	onRejected: {
		console.log("Canceled")
	}
}
