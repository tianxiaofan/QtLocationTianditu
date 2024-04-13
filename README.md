# QtLocationTianditu
QtLocation学习及天地图 插件

+ 子项目Map01 - Map04 为QtLocation的学习过程,如何调用map,文档可查看

csdn:https://blog.csdn.net/ihmhm12345/category_11174178.html

# Map06内包含TiMap天地图项目

+ 支持多图源,具体图源支持在 timapurlengine.cpp 构造函数内查看
+ 支持天地图影像图和影像标注信息,且二者是可以叠加的,叠加方式参见:https://doc.qt.io/qt-5/location-plugin-itemsoverlay.html
+ 支持sqlite数据库存储,原QtLocation地图缓冲是以png图片的方式缓冲到硬盘的,我这里缓冲到sqlite数据库,可以方便管理
+ 只实现了地图的显示,未做地图的搜索,路径等功能
+ Map06目前只支持Qt6.6.2版本,qt6.5和qt6.7暂时未测试
+ Qt6版本使用cmake管理项目
+ 需要Qt5版本的,切换tag 到 qt5.15.2 即可
+ Qt5支持版本使用的pro管理项目

+ 我的CSDN原文链接：https://blog.csdn.net/ihmhm12345/article/details/119078652
