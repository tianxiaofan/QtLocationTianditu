/**************************************************************************
 *   文件名	：tiesrimapprovider.cpp
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
#include "tiesrimapprovider.h"

TiEsriMapProvider::TiEsriMapProvider(const quint32 averageSize, const QGeoMapType::MapStyle mapType,
                                     QObject* parent) :
    TiMapProvider(QString(), averageSize, mapType, parent)
{
}

QNetworkRequest TiEsriMapProvider::getTileURL(const int x, const int y, const int zoom,
                                              QNetworkAccessManager* networkManager)
{
    //-- Build URL
    QNetworkRequest request;
    const QString   url = getURL(x, y, zoom, networkManager);
    if (url.isEmpty()) {
        return request;
    }
    request.setUrl(QUrl(url));
    request.setRawHeader(QByteArrayLiteral("Accept"), QByteArrayLiteral("*/*"));
    const QByteArray token = "AAPK41e218608cc64bbfaecfd4243cb8fe6bkzEVp8H7IfCNN0M-uO-PU0-"
                             "Biah2W8IGndMLDnSBVj98ez-d388bBFoi9XkeXihU";
    request.setRawHeader(QByteArrayLiteral("User-Agent"),
                         QByteArrayLiteral("Qt Location based application"));
    request.setRawHeader(QByteArrayLiteral("User-Token"), token);
    return request;
}

static const QString WorldStreetMapUrl
        = QStringLiteral("http://services.arcgisonline.com/ArcGIS/rest/services/World_Street_Map/"
                         "MapServer/tile/%1/%2/%3");

static const QString WorldSatelliteMapUrl
        = QStringLiteral("http://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/"
                         "MapServer/tile/%1/%2/%3");

static const QString TerrainMapUrl
        = QStringLiteral("http://server.arcgisonline.com/ArcGIS/rest/services/World_Terrain_Base/"
                         "MapServer/tile/%1/%2/%3");

QString TiEsriWorldSatelliteMapProvider::getURL(const int x, const int y, const int zoom,
                                                QNetworkAccessManager* networkManager)
{
    Q_UNUSED(networkManager)
    return WorldSatelliteMapUrl.arg(zoom).arg(y).arg(x);
}

QString TiEsriTerrainMapProvider::getURL(const int x, const int y, const int zoom,
                                         QNetworkAccessManager* networkManager)
{
    Q_UNUSED(networkManager)
    return TerrainMapUrl.arg(zoom).arg(y).arg(x);
}

QString TiEsriWorldStreetMapProvider::getURL(const int x, const int y, const int zoom,
                                             QNetworkAccessManager* networkManager)
{
    Q_UNUSED(networkManager)
    return WorldStreetMapUrl.arg(zoom).arg(y).arg(x);
}
