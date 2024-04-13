import QtQuick
import QtLocation
import QtPositioning

Window {
    visible: true
    width: 800
    height: 600
    Map {
        id:map
        //覆盖parent,采用依靠在bottom,自己去关联map大小
//        anchors.fill: parent
        width: parent.width
        height: parent.height
        anchors.bottom:parent.bottom

        plugin: Plugin { name: "osm" }
        center: QtPositioning.coordinate(34.23,108.87) // Oslo
        zoomLevel: 14

        PinchHandler {
            id: pinch
            target: null
            onActiveChanged: if (active) {
                                 map.startCentroid = map.toCoordinate(pinch.centroid.position, false)
                             }
            onScaleChanged: (delta) => {
                                map.zoomLevel += Math.log2(delta)
                                map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
                            }
            onRotationChanged: (delta) => {
                                   map.bearing -= delta
                                   map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
                               }
            grabPermissions: PointerHandler.TakeOverForbidden
        }
        WheelHandler {
            id: wheel
            // workaround for QTBUG-87646 / QTBUG-112394 / QTBUG-112432:
            // Magic Mouse pretends to be a trackpad but doesn't work with PinchHandler
            // and we don't yet distinguish mice and trackpads on Wayland either
            acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland"
                             ? PointerDevice.Mouse | PointerDevice.TouchPad
                             : PointerDevice.Mouse
            rotationScale: 1/120
            property: "zoomLevel"
        }
        DragHandler {
            id: drag
            target: null
            onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
        }
        Shortcut {
            enabled: map.zoomLevel < map.maximumZoomLevel
            sequence: StandardKey.ZoomIn
            onActivated: map.zoomLevel = Math.round(map.zoomLevel + 1)
        }
        Shortcut {
            enabled: map.zoomLevel > map.minimumZoomLevel
            sequence: StandardKey.ZoomOut
            onActivated: map.zoomLevel = Math.round(map.zoomLevel - 1)
        }

        // The code below enables SSAA
//        layer.enabled: true
//        layer.smooth: true
//        property int w : width
//        property int h : height
//        property int pr: Screen.devicePixelRatio
//        layer.textureSize: Qt.size(w  * 2 * pr, h * 2 * pr)

        layer.enabled: true
        layer.samples: 8

        Component.onCompleted: {
            for(let i = 0;i < supportedMapTypes.length;i++)
            {
                console.log(supportedMapTypes[i].name)
                if(supportedMapTypes[i].name === "World Imagery")
                {
                    activeMapType = supportedMapTypes[i];
                    break
                }
            }
            console.log("activeMapType: " + activeMapType.name)

            //添加一个圆
//            map.addMapItem(circle)
//            map.addMapItem(polygon)
            map.addMapItem(lines)
//            map.addMapItem(rect)
        }
    }

    MapCircle {
        id: circle
        center: QtPositioning.coordinate(34.23,108.87)
        radius: 100
        border.width: 2
        border.color: "green"
//        color: "green"

        MouseArea {
            anchors.fill: parent
            drag.target: parent
        }
    }

    MapPolygon
    {
        id:polygon
        border.width: 3
        color: "green"
        path: [QtPositioning.coordinate(34.23,108.87)
            ,QtPositioning.coordinate(34.23,108.97)
            ,QtPositioning.coordinate(34.63,108.67)
            ,QtPositioning.coordinate(34.33,108.87)]
    }

    MapPolyline{
        id:lines
        line.width: 3
        line.color:  "yellow"
        path: [QtPositioning.coordinate(34.23,108.87)
            ,QtPositioning.coordinate(34.23,108.97)
            ,QtPositioning.coordinate(34.63,108.67)
            ,QtPositioning.coordinate(34.33,108.87)]
    }
    MapRectangle {
        id:rect
        border.width: 3
        color: "red"
        topLeft: QtPositioning.coordinate(34.23,108.87)
        bottomRight: QtPositioning.coordinate(34.33,108.97)
    }
}
