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
    m_providerNameList["Bing Road"] = "";
    m_providerNameList["Bing Satellite"] = "";
    m_providerNameList["Bing Hybrid"] = "";
    m_providerNameList["Google Street Map"] = "";
    m_providerNameList["Google Satellite"] = "";
    m_providerNameList["Google Terrain"] = "";
    m_providerNameList["Google Hybrid"] = "";
    m_providerNameList["Google Labels"] = "";
    m_providerNameList["Tianditu Satellite"] = "Tianditu Satellite Road";
    m_providerNameList["Tianditu Street"] = "Tianditu Street Road";

    m_providerList.append(
        ProviderPair("Tianditu Satellite", new TiTiandituImgMapProvider(this))); //天地图卫星图
    m_providerList.append(ProviderPair("Tianditu Satellite Road",
                                       new TiTiandituCiaMapProvider(this))); //天地图路网标注信息图
    m_providerList.append(
        ProviderPair("Tianditu Street", new TiTiandituVecMapProvider(this))); //失量图
    m_providerList.append(
        ProviderPair("Tianditu Street Road", new TiTiandituCvaMapProvider(this))); //失量图标注

    m_providerList.append(ProviderPair("Google Street Map", new TiGoogleStreetMapProvider(this)));
    m_providerList.append(ProviderPair("Google Satellite", new TiGoogleSatelliteMapProvider(this)));
    m_providerList.append(ProviderPair("Google Terrain", new TiGoogleTerrainMapProvider(this)));
    m_providerList.append(ProviderPair("Google Hybrid", new TiGoogleHybridMapProvider(this)));
    m_providerList.append(ProviderPair("Google Labels", new TiGoogleTerrainMapProvider(this)));

    m_providerList.append(ProviderPair("Bing Road", new TiBingRoadMapProvider(this)));
    m_providerList.append(ProviderPair("Bing Satellite", new TiBingSatelliteMapProvider(this)));
    m_providerList.append(ProviderPair("Bing Hybrid", new TiBingHybridMapProvider(this)));
}

QMap<QString, QString> TiMapUrlEngine::getProviderNameList() const
{
    return m_providerNameList;
}

QString TiMapUrlEngine::getTypeFromId(const int id) const
{
    if (id >= 1 && id <= m_providerList.size()) {
        return m_providerList.at(id - 1).first;
    } else {
        return m_providerList.at(0).first;
    }
}

int TiMapUrlEngine::getIdFromType(const QString& type) const
{
    for (int i = 0; i < m_providerList.size(); i++) {
        if (m_providerList.at(i).first == type) {
            return i + 1;
        }
    }
    return 1;
}

TiMapProvider* TiMapUrlEngine::getMapProviderFromId(int id) const
{
    if (id >= 1 && id <= m_providerList.count()) {
        return m_providerList.at(id - 1).second;
    } else {
        return nullptr;
    }
}

TiMapProvider* TiMapUrlEngine::getMapProviderFromType(const QString& type) const
{
    for (int i = 0; i < m_providerList.size(); i++) {
        if (m_providerList.at(i).first == type) {
            return m_providerList.at(i).second;
        }
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
