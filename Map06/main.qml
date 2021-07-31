import QtQuick 2.12
import QtQuick.Window 2.12
import QtLocation 5.12
import QtPositioning 5.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    Plugin {
        id:mapPlu
        name: "TiMap"
        PluginParameter {
            name: "mapProvider"
            value: "tiandituImg"
        }
        PluginParameter {
            name: "format"
            value: "png"
        }
        PluginParameter{
            name: "cachePath"
            value:"C:/Users/txf/AppData/Local/cache"
        }

    }
    Map {
        id:map
        //覆盖parent,采用依靠在bottom,自己去关联map大小
        width: parent.width
        height: parent.height
        anchors.bottom:parent.bottom

        plugin: mapPlu
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
