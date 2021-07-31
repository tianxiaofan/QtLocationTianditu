import QtQuick 2.0
import QtQuick.Window 2.14
import QtLocation 5.6
import QtPositioning 5.6

Window {
    visible: true
    width: 800
    height: 600
    Map {
        anchors.fill: parent
        plugin: Plugin { name: "esri" }
        center: QtPositioning.coordinate(59.91, 10.75) // Oslo
        zoomLevel: 14
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
        }
    }
}
