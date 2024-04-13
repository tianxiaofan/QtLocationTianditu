import QtQuick
import QtLocation
import QtPositioning

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    // 支持图源： 参见： timapurlengine.cpp
    Map {
        id:map
        //覆盖parent,采用依靠在bottom,自己去关联map大小
        width: parent.width
        height: parent.height
        anchors.bottom:parent.bottom

        plugin:Plugin {
            name: "TiMap"
        }
        center: QtPositioning.coordinate(34.23,108.87) // xi`an
        zoomLevel: 14

        // 可以解决抗锯齿的问题
        // from : https://stackoverflow.com/a/49576756
        layer.enabled: true
        layer.samples: 8

        Component.onCompleted: {
            // 失量图
            updateActiveMapType(map,"Tianditu Satellite");
        }

        //叠加标注信息图层
        Map {
            id: overlay_map
            anchors.fill: parent
            plugin: Plugin {
                name: "TiMap"
            }
            center: parent.center
            color: 'transparent' // Necessary to make this map transparent
            minimumFieldOfView: parent.minimumFieldOfView
            maximumFieldOfView: parent.maximumFieldOfView
            minimumTilt: parent.minimumTilt
            maximumTilt: parent.maximumTilt
            minimumZoomLevel: parent.minimumZoomLevel
            maximumZoomLevel: parent.maximumZoomLevel
            zoomLevel: parent.zoomLevel
            tilt: parent.tilt;
            bearing: parent.bearing
            fieldOfView: parent.fieldOfView
            z: parent.z + 1;
            Component.onCompleted: {
                // 失量图 标注
                updateActiveMapType(this,"Tianditu Satellite Road");
            }

        }

        // 注意： Qt6 中，地图控件不再自带拖拽，缩放等操作，需要手动操作，官方提供以下方式进行地图操作
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

        //注意，如果使用此方式，在地图上层有Rectangle 等控件时，在此控件上操作也会影响到底层的地图，DragHandler暂时无法解决，
        // 如果遇到此问题，可以尝试使用 下方 MouseArea 里的地图拖拽方式，禁用DragHandler
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

/*
        MouseArea {
            id: mapMouseArea
            anchors.fill: parent
            hoverEnabled: true
            z: overlayMap.visible ? overlayMap.z + 1 : map.z + 1
            property real startMouseX: 0
            property real startMouseY: 0
            property real lastMouseX: 0
            property real lastMouseY: 0

            onClicked: mouse => {
                           if (mouse.button === Qt.LeftButton) {
                               let point = Qt.point(mouse.x, mouse.y)
                               // mapPositionClicked(point)
                               // mapCoordinateClicked(map.toCoordinate(point))
                           }
                       }

            onPressed: mouse => {
                           lastMouseX = Math.round(mouse.x)
                           lastMouseY = Math.round(mouse.y)
                           startMouseX = lastMouseX
                           startMouseY = lastMouseY
                       }
            onDoubleClicked: mouse => {
                                 // mapPositionDoubleClicked(Qt.point(mouse.x,
                                 //                                   mouse.y))
                             }

            onPositionChanged: mouse => {
                                   let point = Qt.point(mouse.x, mouse.y)
                                   mapPosition = point
                                   mapCoordinate = map.toCoordinate(point)
                                   if (pressed) {
                                       var newMouseX = Math.round(mouse.x)
                                       var newMouseY = Math.round(mouse.y)
                                       map.pan(lastMouseX - newMouseX,
                                                lastMouseY - newMouseY)
                                       lastMouseX = newMouseX
                                       lastMouseY = newMouseY
                                   }
                               }
        }

*/

        MapCircle {
            id: circle
            center: QtPositioning.coordinate(34.23,108.87)
            z : overlay_map.z + 1
            radius: 500
            border.width: 3
            border.color: "yellow"

            MouseArea {
                anchors.fill: parent
                drag.target: parent
            }
        }
    }




    //更新地图显示类型
    function updateActiveMapType(control,para) {
        for (var i = 0; i < control.supportedMapTypes.length; i++) {
            console.log(control.supportedMapTypes[i].name)
            if (para === control.supportedMapTypes[i].name) {
                control.activeMapType = control.supportedMapTypes[i]
                return
            }
        }
    }
}
