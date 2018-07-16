import QtQuick 2.10
import QtQuick.Dialogs 1.3

MessageDialog {
	id: messageDialog
	text: qsTr("The memory has been modified.")
	informativeText: qsTr("Do you want to save your changes?")
	standardButtons: MessageDialog.Save | MessageDialog.Discard | MessageDialog.Cancel
	property var command

	function doCommand() {
		if(command)
			command()
	}
}
