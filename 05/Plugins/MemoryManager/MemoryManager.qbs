import qbs
import qbs.FileInfo

DynamicLibrary {
	name: "MemoryManager"
	property string uri: "MemoryManager"
	property string globalPath: FileInfo.path(FileInfo.path(sourceDirectory))
	property string globalBinPath: globalPath + "/bin/win32/debug"
	property string globalModulesPath: globalBinPath + "/qml"
	property string globalIncludePath: globalPath + "/Src"

	Depends { name: "cpp" }
	Depends { name: "Qt"; submodules: ["core", "widgets", "quick", "qml"] }

	destinationDirectory: FileInfo.joinPaths(globalModulesPath,prefix,name)

	property string putOverBinSubdir: ""
	property string prefix: ""
	property stringList addIncludes: []
	property stringList defines: ["BUILD_MEMORY_LIB"]
	property stringList staticLibraries: []
	property stringList libraryPaths: []

	cpp.includePaths: [globalPath, globalIncludePath]
	cpp.defines: project.buildWithEasyProfiler ? defines.concat(["BUILD_WITH_EASY_PROFILER"]) : defines
	cpp.cxxLanguageVersion: "c++11"
	//cpp.debugInformation: project.generatePDB
	//cpp.staticLibraries: project.buildWithEasyProfiler ? staticLibraries.concat(["easy_profiler"]) : staticLibraries
	//cpp.libraryPaths: project.buildWithEasyProfiler ? libraryPaths.concat(FileInfo.joinPaths(project.globalProjectsPath,"3rdparty","easy_profiler","lib","win64")) : libraryPaths
	//cpp.optimization: project.generatePDB ? "none" : "fast"

	Group {
		fileTagsFilter: ["dynamiclibrary"]
		qbs.install: true
		qbs.installDir: {
			var p = FileInfo.relativePath(qbs.installRoot, FileInfo.joinPaths(globalBinPath, putOverBinSubdir))
			//console.info(p)
			return p
		}
	}

	Group {
		name: "qmldir"
		files: FileInfo.joinPaths(sourceDirectory,"qmldir")
		qbs.install: true
		qbs.installDir: {
			var p = FileInfo.relativePath(qbs.installRoot, FileInfo.joinPaths(globalBinPath,"qml",product.prefix,product.name))
			//console.info(p)
			return p
		}
	}

	Group {
		name: "src"
		files: [
			"*.cpp",
			"*.h",
			"../../Src/Memory/*.cpp",
			"../../Src/Memory/*.h",
		]
		excludeFiles: "../../Src/Memory/MemoryManager.*"
	}

}
