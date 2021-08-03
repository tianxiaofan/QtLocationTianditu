import QtQuick 2.12
import QtQuick.Window 2.12
import QtLocation 5.12
import QtPositioning 5.12

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

        plugin:Plugin {
            name: "TiMap"
            PluginParameter {
                name: "mapProvider"
                value: "tiandituImg"
            }
            PluginParameter {
                name: "format"
                value: "png"
            }
            //弃用,缓存已使用数据库,代码内默认了数据库存放于程序运行当前目录
            PluginParameter{
                name: "cachePath"
                value:"C:/Users/txf/AppData/Local/cache"
            }

        }
        center: QtPositioning.coordinate(34.23,108.87) // xi`an
        zoomLevel: 14

        // The code below enables SSAA
        layer.enabled: true
        layer.smooth: true
        property int w : width
        property int h : height
        property int pr: Screen.devicePixelRatio
        layer.textureSize: Qt.size(w  * 2 * pr, h * 2 * pr)

        Component.onCompleted: {
            updateActiveMapType(map,"Tianditu Satellite");
        }

        //叠加标注信息图层
        Map {
            anchors.fill: parent
            plugin: Plugin {
                name: "TiMap"
                PluginParameter {
                    name: "mapProvider"
                    value: "tiandituImg"
                }
                PluginParameter {
                    name: "format"
                    value: "png"
                }
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
                updateActiveMapType(this,"Tianditu Street");
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
