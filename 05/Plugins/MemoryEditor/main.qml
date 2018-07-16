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
	title: qsTr("Memory Editor")
	property string appDirPath

	Component.onCompleted: {
		//console.log()
		appDirPath = app.applicationDirPath()
		//memModel.open(appDirPath + "/memory.moi")
	}

	onClosing: {
		close.accepted = false
		fileActions.actionQuit.trigger(mainWindow)
	}

	menuBar: MainMenu {
		actionsFile: fileActions
		actionsEditor: editor.actions
	}

	FileActions {
		id: fileActions
		memModel: memModel
	}

	// Модель
	MemoryModel {
		id: memModel
		property var eMemoryChange: ["mcNone", "mcAdd", "mcAddFrom", "mcDel", "mcEditName", "mcEditVal", "mcUpdate", "mcSelect", "mcClear", "mcMove"]
		onChange: {
			var mct = eMemoryChange[ev.type]
			console.log(mct, ev, ev.me)
		}
	}

	// Диалог Редактора памяти
	MemoryEditorDialog {
		id: editor
		memModel: memModel
	}

	// Запуск сборщика мусора вручную так сказать, но вроде как не нужно
	Timer {
		interval: 1000
		running: false
		repeat: true
		onTriggered: gc()
	}
}
