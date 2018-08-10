import qbs
import qbs.FileInfo

DynamicLibrary {
	name: "PanoramaPlugin"
	property string uri: "PanoramaPlugin"
	property string globalPath: FileInfo.path(FileInfo.path(sourceDirectory))
	property string globalBinPath: FileInfo.joinPaths(globalPath,"bin/win32",qbs.buildVariant)
	property string globalModulesPath: globalBinPath + "/qml"
	property string globalIncludePath: globalPath + "/Src"
	property string globalInterfacesPath: globalPath + "/Interfaces"

	Depends { name: "cpp" }
	Depends { name: "Qt"; submodules: ["core", "quick", "qml"] }

	destinationDirectory: FileInfo.joinPaths(globalModulesPath,prefix,name)

	property string putOverBinSubdir: ""
	property string prefix: ""
	property stringList addIncludes: []
	property stringList defines: ["BUILD_PANORAMAPLUGIN_LIB"]
	property stringList staticLibraries: []
	property stringList libraryPaths: []
	property string installBinDir: {
		var p = FileInfo.relativePath(qbs.installRoot, FileInfo.joinPaths(globalBinPath, putOverBinSubdir))
		console.info(p)
		return p
	}
	property string installQmlDir: {
		var p = FileInfo.relativePath(qbs.installRoot, FileInfo.joinPaths(globalBinPath,"qml",prefix,name))
		console.info(p)
		return p
	}

	cpp.includePaths: [globalPath, globalIncludePath, globalInterfacesPath]
	cpp.defines: project.buildWithEasyProfiler ? defines.concat(["BUILD_WITH_EASY_PROFILER"]) : defines
	cpp.cxxLanguageVersion: "c++11"
	//cpp.debugInformation: project.generatePDB
	//cpp.staticLibraries: project.buildWithEasyProfiler ? staticLibraries.concat(["easy_profiler"]) : staticLibraries
	//cpp.libraryPaths: project.buildWithEasyProfiler ? libraryPaths.concat(FileInfo.joinPaths(project.globalProjectsPath,"3rdparty","easy_profiler","lib","win64")) : libraryPaths
	//cpp.optimization: project.generatePDB ? "none" : "fast"

	Group {
		fileTagsFilter: ["dynamiclibrary"]
		qbs.install: false
		qbs.installDir: installBinDir
	}

	Group {
		name: "qmldir"
		files: FileInfo.joinPaths(sourceDirectory,"qmldir")
		qbs.install: true
		qbs.installDir: installQmlDir
	}

	Group {
		name: "src"
		files: [
			"*.cpp",
			"*.h"
		]
	}

	Group {
		name: "qml"
		files: ["*.qml"]
		qbs.install: true
		qbs.installDir: installQmlDir
	}

}
