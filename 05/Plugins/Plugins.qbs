import qbs
import qbs.FileInfo

Project
{
	property string globalPath: FileInfo.path(FileInfo.path(sourceDirectory))
	property string globalBinPath: FileInfo.joinPaths(globalPath,"bin","win32",qbs.buildVariant)
	property string globalModulesPath: globalBinPath + "/qml"
	property string globalIncludePath: globalPath + "/Src"
	readonly property stringList importPaths: [globalModulesPath]

	references: [
		"MemoryManager/MemoryManager.qbs",
		"MemoryEditor/MemoryEditor.qbs"
	]
}
