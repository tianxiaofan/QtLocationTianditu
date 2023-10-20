/**************************************************************************
 *   文件名	：tibingmapprovider.cpp
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-8-31
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
#include "tibingmapprovider.h"

TiBingMapProvider::TiBingMapProvider(const QString& imageFormat, const quint32 averageSize,
                                     const QGeoMapType::MapStyle mapType, QObject* parent) :
    TiMapProvider(imageFormat, averageSize, mapType, parent)
{
}

static const QString RoadMapUrl
        = QStringLiteral("http://ecn.t%1.tiles.virtualearth.net/tiles/r%2.png?g=%3&mkt=%4");

QString TiBingRoadMapProvider::getURL(const int x, const int y, const int zoom,
                                      QNetworkAccessManager* networkManager)
{
    Q_UNUSED(networkManager)
    const QString key = tileXYToQuadKey(x, y, zoom);
    return RoadMapUrl.arg(QString::number(getServerNum(x, y, 4)), key, _versionBingMaps, m_language);
}

static const QString SatteliteMapUrl
        = QStringLiteral("http://ecn.t%1.tiles.virtualearth.net/tiles/a%2.jpeg?g=%3&mkt=%4");

QString TiBingSatelliteMapProvider::getURL(const int x, const int y, const int zoom,
                                           QNetworkAccessManager* networkManager)
{
    Q_UNUSED(networkManager)
    const QString key = tileXYToQuadKey(x, y, zoom);
    return SatteliteMapUrl.arg(QString::number(getServerNum(x, y, 4)), key, _versionBingMaps,
                               m_language);
}

static const QString HybridMapUrl
        = QStringLiteral("http://ecn.t%1.tiles.virtualearth.net/tiles/h%2.jpeg?g=%3&mkt=%4");

QString TiBingHybridMapProvider::getURL(const int x, const int y, const int zoom,
                                        QNetworkAccessManager* networkManager)
{
    Q_UNUSED(networkManager)
    const QString key = tileXYToQuadKey(x, y, zoom);
    return HybridMapUrl.arg(QString::number(getServerNum(x, y, 4)), key, _versionBingMaps, m_language);
}
