import qbs
import qbs.FileInfo
import "../cppplugin.qbs" as Plugin

Plugin {
	name: "MemoryManager"
	property string uri: "MemoryManager"
	installIntoBin: true

	Depends { name: "Qt.widgets" }
	defines: ["BUILD_MEMORY_LIB"]

	Group {
		name: "src_memory"
		files: [
			FileInfo.joinPaths(globalSourcePath, "Memory/**")
		]
	}

}
