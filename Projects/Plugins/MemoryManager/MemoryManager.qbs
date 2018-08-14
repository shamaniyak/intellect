import qbs
import qbs.FileInfo

DynamicLibrary {
	name: "MemoryManager"
	property string uri: "MemoryManager"
	property string globalPath: project.globalPath
	property string globalBinPath: project.globalBinPath
	property string globalModulesPath: project.globalModulesPath
	property string globalIncludePath: project.globalIncludePath

	Depends { name: "cpp" }
	Depends { name: "Qt"; submodules: ["core", "widgets", "quick", "qml"] }

	destinationDirectory: FileInfo.joinPaths(globalModulesPath,prefix,name)

	property string putOverBinSubdir: ""
	property string prefix: ""
	property stringList addIncludes: []
	property stringList defines: ["BUILD_MEMORY_LIB"]
	property stringList staticLibraries: []
	property stringList libraryPaths: []
	property string easyProfDefine: project.buildWithEasyProfiler ? "BUILD_WITH_EASY_PROFILER" : ""

	cpp.includePaths: [globalIncludePath]
	cpp.defines: defines.concat([easyProfDefine])
	cpp.cxxLanguageVersion: "c++11"
	//cpp.debugInformation: project.generatePDB
	//cpp.staticLibraries: project.buildWithEasyProfiler ? staticLibraries.concat(["easy_profiler"]) : staticLibraries
	//cpp.libraryPaths: project.buildWithEasyProfiler ? libraryPaths.concat(FileInfo.joinPaths(project.globalProjectsPath,"3rdparty","easy_profiler","lib","win64")) : libraryPaths
	//cpp.optimization: project.generatePDB ? "none" : "fast"

	Group {
		fileTagsFilter: ["dynamiclibrary"]
		qbs.install: true
		qbs.installDir: {
			var p = FileInfo.relativePath(qbs.installRoot, globalBinPath)
			//console.info(p)
			return p
		}
	}

	Group {
		name: "qmldir"
		files: ["qmldir"]
		qbs.install: true
		qbs.installDir: {
			var p = FileInfo.relativePath(qbs.installRoot, destinationDirectory)
			//console.info(p)
			return p
		}
	}

	Group {
		name: "src"
		files: [
			"*.cpp",
			"*.h",
			FileInfo.joinPaths(globalIncludePath, "Memory/**")
		]
	}

}
