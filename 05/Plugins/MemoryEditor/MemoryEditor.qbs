import qbs
import qbs.FileInfo

Product {

	property string globalPath: FileInfo.path(sourceDirectory)
	property string globalBinPath: globalPath + "/bin"
	property string globalModulesPath: globalBinPath + "/qml"
	property string globalIncludePath: globalPath + "/Src"

	Group {
		name: "MemoryEditor"
		files: [
			"qmldir",
			"*.qml"
		]
	}
}
