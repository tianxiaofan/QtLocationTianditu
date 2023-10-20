/**************************************************************************
 *   文件名	：tibingmapprovider.h
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

class TiBingMapProvider : public TiMapProvider
{
    Q_OBJECT

public:
    TiBingMapProvider(const QString& imageFormat, const quint32 averageSize,
                      const QGeoMapType::MapStyle mapType, QObject* parent = nullptr);

    ~TiBingMapProvider() = default;

protected:
    const QString _versionBingMaps = QStringLiteral("563");
};

static const quint32 AVERAGE_BING_STREET_MAP = 1297;
static const quint32 AVERAGE_BING_SAT_MAP    = 19597;

// -----------------------------------------------------------
// Bing Road Map

class TiBingRoadMapProvider : public TiBingMapProvider
{
    Q_OBJECT

public:
    TiBingRoadMapProvider(QObject* parent = nullptr) :
        TiBingMapProvider(QStringLiteral("png"), AVERAGE_BING_STREET_MAP, QGeoMapType::StreetMap, parent)
    {
    }

    QString getURL(const int x, const int y, const int zoom,
                   QNetworkAccessManager* networkManager) override;
};

// -----------------------------------------------------------
// Bing Satellite Map

class TiBingSatelliteMapProvider : public TiBingMapProvider
{
    Q_OBJECT

public:
    TiBingSatelliteMapProvider(QObject* parent = nullptr) :
        TiBingMapProvider(QStringLiteral("jpg"), AVERAGE_BING_SAT_MAP, QGeoMapType::SatelliteMapDay,
                          parent)
    {
    }

    QString getURL(const int x, const int y, const int zoom,
                   QNetworkAccessManager* networkManager) override;
};

// -----------------------------------------------------------
// Bing Hybrid Map

class TiBingHybridMapProvider : public TiBingMapProvider
{
    Q_OBJECT
public:
    TiBingHybridMapProvider(QObject* parent = nullptr) :
        TiBingMapProvider(QStringLiteral("jpg"), AVERAGE_BING_SAT_MAP, QGeoMapType::HybridMap, parent)
    {
    }

    QString getURL(const int x, const int y, const int zoom,
                   QNetworkAccessManager* networkManager) override;
};
