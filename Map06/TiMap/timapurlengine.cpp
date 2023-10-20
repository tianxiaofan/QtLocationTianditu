/**************************************************************************
 *   文件名	：timapurlengine.cpp
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-6-17
 *   邮   箱	：499131808@qq.com
 *   Q Q		：499131808
 *   公   司      ：
 *   功能描述      ：
 *   
 *   ======================================================================
 *   修改者	：
 *   修改日期	：
 *   修改内容	：
 *   ======================================================================
 *
 ***************************************************************************/
#include "timapurlengine.h"
#include "titianditumapprovider.h"
#include "tigooglemapprovider.h"
#include "tiesrimapprovider.h"
#include "tibingmapprovider.h"
#include "tilogger.h"

TiMapUrlEngine::TiMapUrlEngine(QObject* parent) : QObject(parent)
{
    m_provides["Tianditu Img"] = new TiTiandituImgMapProvider(this); //天地图卫星图
    m_provides["Tianditu Cia"] = new TiTiandituCiaMapProvider(this); //天地图路网标注信息图
    m_provides["Tianditu Vec"] = new TiTiandituVecMapProvider(this); //失量图
    m_provides["Tianditu Cva"] = new TiTiandituCvaMapProvider(this); //失量图标注

    m_provides["Google Street Map"] = new TiGoogleStreetMapProvider(this);
    m_provides["Google Satellite"]  = new TiGoogleSatelliteMapProvider(this);
    m_provides["Google Terrain"]    = new TiGoogleTerrainMapProvider(this);
    m_provides["Google Hybrid"]     = new TiGoogleHybridMapProvider(this);
    m_provides["Google Labels"]     = new TiGoogleTerrainMapProvider(this);

    m_provides["Esri World Street"]    = new TiEsriWorldStreetMapProvider(this);
    m_provides["Esri World Satellite"] = new TiEsriWorldSatelliteMapProvider(this);
    m_provides["Esri Terrain"]         = new TiEsriTerrainMapProvider(this);

    m_provides["Bing Road"]      = new TiBingRoadMapProvider(this);
    m_provides["Bing Satellite"] = new TiBingSatelliteMapProvider(this);
    m_provides["Bing Hybrid"]    = new TiBingHybridMapProvider(this);
}

QStringList TiMapUrlEngine::getProviderNameList() const
{
    return m_provides.keys();
}

QString TiMapUrlEngine::getTypeFromId(const int id) const
{
    auto i = m_provides.constBegin();
    while (i != m_provides.constEnd()) {
        if ((int)(qHash(i.key()) >> 1) == id)
            return i.key();
        ++i;
    }
    return QString();
}

// qhash为uint,转int会超int范围,右移一位缩小一倍
int TiMapUrlEngine::getIdFromType(const QString& type) const
{
    return (int)(qHash(type) >> 1);
}

TiMapProvider* TiMapUrlEngine::getMapProviderFromId(int id) const
{
    QString _type = getTypeFromId(id);
    return getMapProviderFromType(_type);
}

TiMapProvider* TiMapUrlEngine::getMapProviderFromType(const QString& type) const
{
    if (!type.isEmpty()) {
        if (m_provides.find(type) != m_provides.end())
            return m_provides[type];
    }
    return nullptr;
}

QNetworkRequest TiMapUrlEngine::getTileUrl(int id, int x, int y, int zoom,
                                           QNetworkAccessManager* networkManager) const
{
    auto _provide = getMapProviderFromId(id);
    if (_provide)
        return _provide->getTileURL(x, y, zoom, networkManager);

    LOG_WARN() << "map not registered:" << getTypeFromId(id);
    return QNetworkRequest(QUrl());
}

QNetworkRequest TiMapUrlEngine::getTileUrl(const QString& type, int x, int y, int zoom,
                                           QNetworkAccessManager* networkManager) const
{
    auto _provide = getMapProviderFromType(type);
    if (_provide)
        return _provide->getTileURL(x, y, zoom, networkManager);
    LOG_WARN() << "map not registered:" << type;
    return QNetworkRequest(QUrl());
}

QString TiMapUrlEngine::getImageFormat(QString type, const QByteArray& image)
{
    auto _provide = getMapProviderFromType(type);
    if (_provide) {
        return _provide->getImageFormat(image);
    }
    LOG_WARN() << "map not find ByteArray format";
    return "";
}

QString TiMapUrlEngine::getImageFormat(int id, const QByteArray& image)
{
    auto _provide = getMapProviderFromId(id);
    if (_provide) {
        return _provide->getImageFormat(image);
    }
    LOG_WARN() << "map not find ByteArray format";
    return "";
}

/**
 * @brief TiMapUrlEngine::isElevation 是否是高程数据
 * @param mapId
 * @return 如未获取到,则默认返回false
 */
bool TiMapUrlEngine::isElevation(int mapId)
{
    auto _provide = getMapProviderFromId(mapId);
    if (_provide)
        return _provide->isElevationProvider();
    return false;
}
