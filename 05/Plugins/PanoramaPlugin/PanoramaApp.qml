import QtQuick 2.10
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import PanoramaPlugin 1.0

ApplicationWindow {
	id: app
	visible: true
	width: 1000
	height: 800

	Flickable {
		id: mapView
		anchors.fill: parent
		contentWidth: map.width
		contentHeight: map.height
		function center() {
			var centrePoint = Qt.point(contentX + width/2, contentY+height/2)
			IObj.log(centrePoint)
			var bl = map.xy_BL(centrePoint.x, centrePoint.y)
			return bl
		}
		Panorama {
			id: map

			Component.onCompleted: {
				setFileMap("C:/Work/TestCartograf/Maps/World/world.map")
			}

			MouseArea {
				anchors.fill: parent
				hoverEnabled: true
				//enabled: !mapPinchArea.pinch.active

				onPositionChanged: {
					pos.x = mouse.x
					pos.y = mouse.y
					var bl = map.xy_BL(mouse.x, mouse.y)
					pos.b = bl.x
					pos.l = bl.y
					//IObj.log(bl)
				}
			}

			MultiPointTouchArea {
				enabled: false
				TouchPoint { id: point1 }
				TouchPoint { id: point2 }
			}

			PinchArea {
				id: mapPinchArea
				anchors.fill: parent
				pinch.target: map
				enabled: false

				onPinchStarted: {
					IObj.log("pinchStarted")
				}
			}
		}
	}

	Column {
		anchors.right: parent.right
		anchors.verticalCenter: parent.verticalCenter
		width: 40
		Button {
			width: parent.width
			height: 40
			text: "+"
			onClicked: {
				var center = mapView.center()
				map.zoomIn(center.x, center.y)
			}
		}
		Button {
			width: parent.width
			height: 40
			text: "-"
			onClicked: {
				var center = mapView.center()
				map.zoomOut(center.x, center.y)
			}
		}
	}

	QtObject {
		id: pos
		property real x
		property real y
		property real b
		property real l
	}

	// StatusBar
	Item {
		width: parent.width
		height: 40
		anchors.bottom: parent.bottom

		Text {
			text: "X = " + pos.x + ", Y = " + pos.y + ", B = " + (pos.b*180/Math.PI).toFixed(3) + ", L = " + (pos.l*180/Math.PI).toFixed(3)
			anchors.fill: parent
			verticalAlignment: Text.AlignVCenter
			//horizontalAlignment: Text.AlignHCenter
		}
	}

}
