import qbs
import qbs.FileInfo

Product {
	name: "MemoryEditor"
	property string uri: "MemoryEditor"
	property string globalPath: project.globalPath
	property string globalBinPath: project.globalBinPath
	property string globalModulesPath: project.globalModulesPath
	property string globalIncludePath: project.globalIncludePath

	destinationDirectory: FileInfo.joinPaths(globalModulesPath,name)

	Group {
		name: "qml"
		files: [
			"qmldir",
			"*.qml"
		]
		qbs.install: true
		qbs.installDir: FileInfo.relativePath(qbs.installRoot, destinationDirectory)
	}
}
