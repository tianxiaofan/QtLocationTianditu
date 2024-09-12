# QtLocationTianditu
QtLocation学习及天地图 插件

+ 现支持地图缓存且支持多种图源

支持Bing Road 、Bing Satellite、Bing Hybrid、Google Street Map、Google Satellite、Google Terrain、Google Hybrid、Google Labels、Tianditu Satellite、Tianditu Satellite Road、Tianditu Street、Tianditu Street Road

具体可以在timapurlengine.cpp内 构造函数内查看、也可在main.qml中的Map组件下，遍历Map组件的supportedMapTypes属性查看

google的某些图源可能会看不了，目前bing地图，天地图四个图源、google卫星是可用的