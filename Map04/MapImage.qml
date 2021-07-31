import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6
import QtGraphicalEffects 1.12

//图片
MapQuickItem{
    property string imagePath;//设置图片路径
    property real imageAngle: 0 //设置图片angle
    property color imageColor; //更改图片颜色
    property var imageCoord: QtPositioning.coordinate(); //设置图片要显示的位置
    id:imgItem
    sourceItem:Image {
        id: img
        width: 100
        height: 100
        source: imagePath

        //旋转
        transform:Rotation{
            origin.x: img.width / 2
            origin.y: img.height / 2
            angle: imageAngle
        }

        //支持更改图片颜色
        layer.enabled: true;
        layer.effect: ColorOverlay{
            anchors.fill:img
            source: img
            color:imageColor;
            cached:false//默认禁用,如果要启用了会占用大量cpu
        }
    }
    coordinate: imageCoord
    anchorPoint:Qt.point(img.width/2, img.height/2);
}
