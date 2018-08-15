import qbs
import qbs.FileInfo
import "../cppplugin.qbs" as Plugin

Plugin {
	name: "PanoramaPlugin"

	property string uri: "PanoramaPlugin"

	Properties {
		condition: true
		defines: outer.concat("BUILD_PANORAMAPLUGIN_LIB")
	}

}
