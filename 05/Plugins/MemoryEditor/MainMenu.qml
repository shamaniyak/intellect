import QtQuick 2.0
import QtQuick.Controls 2.3

MenuBar {
	property var actionsFile
	property var actionsEditor

	Menu {
		id: fileMenu
		title: qsTr("&File")
		MenuItem { text: qsTr("&New..."); action: actionsFile.actionNew }
		MenuItem { text: qsTr("&Open..."); action: actionsFile.actionOpen }
		MenuItem { text: qsTr("&Save"); action: actionsFile.actionSave }
		MenuItem { text: qsTr("Save &As..."); action: actionsFile.actionSaveAs }
		MenuItem { text: qsTr("Garbage Collector"); action: actionsFile.actionGC }
		MenuItem { text: qsTr("Run Tests"); action: actionsFile.actionTest }
		MenuSeparator { }
		MenuItem { text: qsTr("&Quit"); action: actionsFile.actionQuit }
	}

	Menu {
		id: editorMenu
		title: qsTr("&Edit")
		MenuItem { action: actionsEditor.addAction }
		MenuItem { action: actionsEditor.removeAction }
		MenuItem { action: actionsEditor.clearAction }
	}
}
