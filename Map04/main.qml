import QtQuick
import QtLocation
import QtPositioning

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    Map {
        id:map
        //覆盖parent,采用依靠在bottom,自己去关联map大小
        width: parent.width
        height: parent.height
        anchors.bottom:parent.bottom

        plugin: Plugin { name: "osm" }
        center: QtPositioning.coordinate(34.23,108.87) // xi`an
        zoomLevel: 14

        // The code below enables SSAA
        layer.enabled: true
        layer.smooth: true
        property int w : width
        property int h : height
        property int pr: Screen.devicePixelRatio
        layer.textureSize: Qt.size(w  * 2 * pr, h * 2 * pr)

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
//            addMapItem(image)
            addMapItem(_text)
        }
    }

    MapImage{
        id:image
        imagePath: "marker2.png"
        imageCoord: QtPositioning.coordinate(34.23,108.87)
        imageAngle: 90
        imageColor: "green"
    }
    MapText{
        id:_text
        mapText: "我是测试文字"
        mapCoord: QtPositioning.coordinate(34.23,108.87)
    }

}

