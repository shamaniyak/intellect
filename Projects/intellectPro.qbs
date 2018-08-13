import qbs
import qbs.FileInfo

Project {
	references: [
		"../intellect/intellect.qbs",
		"../plugins/memorymanager/memorymanager.qbs",
		"../plugins/memoryeditor/memoryeditor.qbs"
	]
}
