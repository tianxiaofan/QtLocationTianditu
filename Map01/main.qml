import QtQuick 2.0
import QtQuick.Window 2.14
import QtLocation 6.5
import QtPositioning 6.5

Window {
    visible: true
    width: 800
    height: 600

    Plugin {
        id: mapPlugin
        name: "osm" // "mapboxgl", "esri", ...
        // specify plugin parameters if necessary
        // PluginParameter {
        //     name:
        //     value:
        // }
    }

    Map {
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(59.91, 10.75) // Oslo
        zoomLevel: 14
    }
}
