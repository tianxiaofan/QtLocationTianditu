import QtQuick 2.15
import QtQuick.Window 2.15
import QtLocation 5.6
import QtPositioning 5.6

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

        plugin: Plugin { name: "esri" }
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
        imagePath: "qrc:/marker 2.png"
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

