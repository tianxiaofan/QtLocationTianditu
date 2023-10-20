import QtQuick 2.12
import QtQuick.Window 2.12
import QtLocation 5.12
import QtPositioning 5.12

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

        // from : https://stackoverflow.com/a/49576756
        layer.enabled: true
        layer.samples: 8

        Component.onCompleted: {
            // 卫星图
//            updateActiveMapType(map,"Tianditu Img");
            // 失量图
            updateActiveMapType(map,"Tianditu Vec");
        }

        //叠加标注信息图层
        Map {
            anchors.fill: parent
            plugin: Plugin {
                name: "TiMap"
            }
            gesture.enabled: false
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
                // 卫星图 标注
//                updateActiveMapType(this,"Tianditu Cia");
                // 失量图 标注
                updateActiveMapType(this,"Tianditu Cva");
            }

        }

        MapCircle {
            id: circle
            center: QtPositioning.coordinate(34.23,108.87)
            radius: 500
            border.width: 3
            border.color: "yellow"
            layer.enabled: true
            layer.smooth: true
            property int w : width
            property int h : height
            property int pr: Screen.devicePixelRatio
            layer.textureSize: Qt.size(w  * 2 * pr, h * 2 * pr)

            MouseArea {
                anchors.fill: parent
                drag.target: parent
            }
        }
    }




    //更新地图显示类型
    function updateActiveMapType(control,para) {
        for (var i = 0; i < control.supportedMapTypes.length; i++) {
            if (para === control.supportedMapTypes[i].name) {
                control.activeMapType = control.supportedMapTypes[i]
                return
            }
        }
    }
}
