import qbs
import qbs.FileInfo

Product {

	property string globalPath: FileInfo.path(FileInfo.path(sourceDirectory))
	property string globalBinPath: FileInfo.joinPaths(globalPath,"bin","win32",qbs.buildVariant)
	property string globalModulesPath: globalBinPath + "/qml"
	property string globalIncludePath: globalPath + "/Src"
	readonly property stringList qmlImportPaths: [globalModulesPath]

	Group {
		name: "MemoryEditor"
		files: [
			"qmldir",
			"*.qml"
		]
		qbs.install: true
		qbs.installDir: FileInfo.joinPaths("bin","win32",qbs.buildVariant,"qml","MemoryEditor")
	}
}
