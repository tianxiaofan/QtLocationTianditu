# QtLocationTianditu
QtLocation学习及天地图 插件

子项目Map01 - Map05 为QtLocation的学习过程,如何调用map,文档可查看

csdn:https://blog.csdn.net/ihmhm12345/category_11174178.html

Map06为天地图项目

类调用关系
TiQeoServiceProviderFactory —> TiQGeoTiledMappingManagerEngine ->> TiGeoTileFetcher
------------------------------------------------------------------------------------------------->>TiQGeoFileTileCache

TiQeoServiceProviderFactory :为插件类,入口
TiQGeoTiledMappingManagerEngine :地图tile 瓦片的管理类
TiGeoTileFetcher : 从网络获取tile的类
TiQGeoFileTileCache: 缓冲到本地文件的类,官方默认的是 QGeoFileTileCache,此会tile以png的形式写入到本地磁盘,默认在C盘目录,我重载重写里面的 insert() 和 get() 函数,写入到数据库

另外有个类TiMapEngine 为单例类,里面有各地图服务的url获取方式,getUrlEngine()及数据库线程getSqlThread()
其中TiMapUrlEngine 类可添加新地图服务,用于支持其它地图,注意现在天地图里面的key是我自己申请的key,每天只有1W张图,自己拿到工作请到天地图官方申请帐号,更换key 位置: titianditumapprovider.cpp

支持天地图影像图和影像标注信息,且二者是可以叠加的,叠加方式参见:https://doc.qt.io/qt-5/location-plugin-itemsoverlay.html
支持sql数据库存储
只实现了地图的显示,未做地图的搜索,路径等功能
Qt5.15.0版本上运行,理论上5.12版本后都可以运行,印象中从5.11后,Qtlocation里就没功能变动?Qt 6目前不支持,主要是官方说Qt6不支持Qtlocation,看后续6.3版本后会不会添加进来
————————————————
版权声明：本文为CSDN博主「ihmhm12345」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/ihmhm12345/article/details/119078652