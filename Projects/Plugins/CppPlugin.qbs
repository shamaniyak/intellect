import qbs
import qbs.FileInfo

DynamicLibrary {
	name: "Plugin"

	property string globalPath: project.globalPath
	property string globalBinPath: project.globalBinPath
	property string globalModulesPath: project.globalModulesPath
	property string globalSourcePath: project.globalSourcePath
	property string globalInterfacesPath: FileInfo.joinPaths(globalSourcePath, "Interfaces")
	property stringList addIncludes: []
	property stringList defines: []
	property stringList staticLibraries: []
	property stringList libraryPaths: []
	property bool installIntoBin: false

	destinationDirectory: FileInfo.joinPaths(globalModulesPath,name)

	Depends { name: "cpp" }
	Depends { name: "Qt"; submodules: ["core", "quick", "qml"] }

	Properties {
		condition: project.buildWithEasyProfiler
		defines: outer.concat("BUILD_WITH_EASY_PROFILER")
	}

	cpp.includePaths: [globalSourcePath, globalInterfacesPath]
	cpp.defines: defines
	cpp.cxxLanguageVersion: "c++11"
	//cpp.debugInformation: project.generatePDB
	//cpp.staticLibraries: project.buildWithEasyProfiler ? staticLibraries.concat(["easy_profiler"]) : staticLibraries
	//cpp.libraryPaths: project.buildWithEasyProfiler ? libraryPaths.concat(FileInfo.joinPaths(project.globalProjectsPath,"3rdparty","easy_profiler","lib","win64")) : libraryPaths
	//cpp.optimization: project.generatePDB ? "none" : "fast"

	Group {
		fileTagsFilter: ["dynamiclibrary"]
		qbs.install: installIntoBin
		qbs.installDir: {
			var p = FileInfo.relativePath(qbs.installRoot, globalBinPath)
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
			FileInfo.joinPaths(sourceDirectory, "*.cpp"),
			FileInfo.joinPaths(sourceDirectory, "*.h")
		]
	}

}
