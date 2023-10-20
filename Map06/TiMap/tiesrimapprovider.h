/**************************************************************************
 *   文件名	：tiesrimapprovider.h
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
#pragma once
#include "timapprovider.h"

class TiEsriMapProvider : public TiMapProvider
{
    Q_OBJECT
public:
    TiEsriMapProvider(const quint32 averageSize, const QGeoMapType::MapStyle mapType,
                      QObject* parent = nullptr);
    QNetworkRequest getTileURL(const int x, const int y, const int zoom,
                               QNetworkAccessManager* networkManager) override;
};

class TiEsriWorldStreetMapProvider : public TiEsriMapProvider
{
    Q_OBJECT

public:
    TiEsriWorldStreetMapProvider(QObject* parent = nullptr) :
        TiEsriMapProvider(AVERAGE_TILE_SIZE, QGeoMapType::StreetMap, parent)
    {
    }

    QString getURL(const int x, const int y, const int zoom,
                   QNetworkAccessManager* networkManager) override;
};

class TiEsriWorldSatelliteMapProvider : public TiEsriMapProvider
{
    Q_OBJECT

public:
    TiEsriWorldSatelliteMapProvider(QObject* parent = nullptr) :
        TiEsriMapProvider(AVERAGE_TILE_SIZE, QGeoMapType::SatelliteMapDay, parent)
    {
    }

    QString getURL(const int x, const int y, const int zoom,
                   QNetworkAccessManager* networkManager) override;
};

class TiEsriTerrainMapProvider : public TiEsriMapProvider
{
    Q_OBJECT

public:
    TiEsriTerrainMapProvider(QObject* parent = nullptr) :
        TiEsriMapProvider(AVERAGE_TILE_SIZE, QGeoMapType::TerrainMap, parent)
    {
    }

    QString getURL(const int x, const int y, const int zoom,
                   QNetworkAccessManager* networkManager) override;
};
