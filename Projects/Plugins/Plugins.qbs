import qbs
import qbs.FileInfo

Project
{
	property string globalPath: {
		var p = FileInfo.path(sourceDirectory)
		//console.info(p)
		return p
	}
	property string globalBinPath: FileInfo.joinPaths(globalPath,"bin","win32",qbs.buildVariant)
	property string globalModulesPath: globalBinPath + "/qml"
	property string globalSourcePath: sourceDirectory + "/Src"
	readonly property stringList importPaths: [globalModulesPath]
	readonly property stringList qmlImportPaths: [globalModulesPath]

	references: [
		"MemoryManager/MemoryManager.qbs",
		"MemoryEditor/MemoryEditor.qbs",
		"PanoramaPlugin/PanoramaPlugin.qbs"
	]
}
