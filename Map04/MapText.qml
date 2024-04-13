import QtQuick
import QtLocation
import QtPositioning
import Qt5Compat.GraphicalEffects
//图片
MapQuickItem{
    property string mapText;//
    property var mapCoord: QtPositioning.coordinate();
    id:imgItem
    sourceItem:Rectangle{
        id:rect
        width:100
        height:30
        border.color:"gray"
        Text {
            id: txt
            text: mapText
            anchors.verticalCenter:parent.verticalCenter;
        }
    }
    coordinate: mapCoord
    anchorPoint:Qt.point(rect.width/2, rect.height/2);
    zoomLevel: 15
}
