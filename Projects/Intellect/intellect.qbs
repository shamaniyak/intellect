import qbs
import qbs.FileInfo

CppApplication {
	name: "Intellect"
	property string globalPath: project.globalPath
	property string globalBinPath: project.globalBinPath
	property string globalModulesPath: project.globalModulesPath
	property string globalIncludePath: project.globalIncludePath
	property string projectsPath: project.sourceDirectory

	Depends { name: "cpp" }
	Depends { name: "Qt"; submodules: ["core", "gui", "widgets", "quick", "qml"] }
	Depends { name: "MemoryManager" }

	destinationDirectory: globalBinPath

	property string putOverBinSubdir: ""
	property string prefix: ""
	property stringList addIncludes: [projectsPath,sourceDirectory,"gui"]
	property stringList defines: []
	property stringList staticLibraries: []
	property stringList libraryPaths: []

	Properties {
		condition: project.buildWithEasyProfiler
		defines: outer.concat("BUILD_WITH_EASY_PROFILER")
	}

	cpp.includePaths: addIncludes.concat([globalIncludePath])
	cpp.defines: defines
	cpp.cxxLanguageVersion: "c++11"
	//cpp.debugInformation: project.generatePDB
	//cpp.staticLibraries: project.buildWithEasyProfiler ? staticLibraries.concat(["easy_profiler"]) : staticLibraries
	//cpp.libraryPaths: project.buildWithEasyProfiler ? libraryPaths.concat(FileInfo.joinPaths(project.globalProjectsPath,"3rdparty","easy_profiler","lib","win64")) : libraryPaths
	//cpp.optimization: project.generatePDB ? "none" : "fast"

	Group {
		condition: false
		fileTagsFilter: ["application"]
		qbs.install: true
		qbs.installDir: {
			var p = FileInfo.relativePath(qbs.installRoot, FileInfo.joinPaths(globalBinPath, putOverBinSubdir))
			//console.info(p)
			return p
		}
	}

	Group {
		name: "src"
		files: [
			"../Src/iobject.*",
			"../src/objectmanager.*",
			"../src/pluginmanager.*",
			"../src/qqmlwrapper.*",
			"../src/scriptwrapper.*",
			"../src/swmrg.*",
			"../src/talgorithm.*",
			"../src/windowmanager.*",
			"../src/commander.*",
			"../src/qmlmanager/*",
			"../src/gui/**",
			"../src/plugins/qmlregistertypes.*",
			"../Third/qmlcreator/cpp/*.*",
			"src/*.cpp",
			"src/*.h",
			"gui/*.cpp",
			"gui/*.h"
		]
		excludeFiles: [
			"gui/formMemoryEditor.*"
		]
	}

	Group {
		name: "UI"
		files: [
			"gui/dialogmemoryeditor.ui",

		]
	}

	Group {
		name: "dictionaries.qrc"
		files: [
			"../../resources/dictionaries.qrc"
		]
	}
}
