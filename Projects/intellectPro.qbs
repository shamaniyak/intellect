import qbs
import qbs.FileInfo

Project {
	property string globalPath: FileInfo.path(sourceDirectory)
	property string globalBinPath: FileInfo.joinPaths(globalPath,"bin/win32",qbs.buildVariant)
	property string globalModulesPath: FileInfo.joinPaths(globalBinPath, "qml")
	property string globalIncludePath: FileInfo.joinPaths(sourceDirectory, "src")
	readonly property stringList importPaths: [globalModulesPath]

	references: [
		"intellect/intellect.qbs",
		"plugins/plugins.qbs"
	]
}
