/**************************************************************************
 *   文件名	：tigooglemapprovider.h
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-8-30
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

#include <QMutex>
#include <QNetworkReply>

class TiGoogleMapProvider : public TiMapProvider
{
    Q_OBJECT
public:
    TiGoogleMapProvider(const QString& format, const quint32 averageSize,
                        const QGeoMapType::MapStyle mapType = QGeoMapType::CustomMap,
                        QObject*                    parent  = nullptr);

    // Google Specific private slots
private slots:
    void _networkReplyError(QNetworkReply::NetworkError error);
    void _googleVersionCompleted();
    void _replyDestroyed();

protected:
    // Google Specific private methods
    void getSecGoogleWords(const int x, const int y, QString& sec1, QString& sec2) const;
    void tryCorrectGoogleVersions(QNetworkAccessManager* networkManager);

    // Google Specific attributes
    bool           _googleVersionRetrieved;
    QNetworkReply* _googleReply;
    QMutex         _googleVersionMutex;
    QString        _versionGoogleMap;
    QString        _versionGoogleSatellite;
    QString        _versionGoogleLabels;
    QString        _versionGoogleTerrain;
    QString        _versionGoogleHybrid;
    QString        _secGoogleWord;
};

static const quint32 AVERAGE_GOOGLE_STREET_MAP  = 4913;
static const quint32 AVERAGE_GOOGLE_SAT_MAP     = 56887;
static const quint32 AVERAGE_GOOGLE_TERRAIN_MAP = 19391;

// -----------------------------------------------------------
// Google Street Map

class TiGoogleStreetMapProvider : public TiGoogleMapProvider
{
    Q_OBJECT

public:
    TiGoogleStreetMapProvider(QObject* parent = nullptr) :
        TiGoogleMapProvider(QStringLiteral("png"), AVERAGE_GOOGLE_STREET_MAP,
                            QGeoMapType::StreetMap, parent)
    {
    }

protected:
    QString getURL(const int x, const int y, const int zoom,
                   QNetworkAccessManager* networkManager) override;
};

// -----------------------------------------------------------
// Google Street Map

class TiGoogleSatelliteMapProvider : public TiGoogleMapProvider
{
    Q_OBJECT

public:
    TiGoogleSatelliteMapProvider(QObject* parent = nullptr) :
        TiGoogleMapProvider(QStringLiteral("jpg"), AVERAGE_GOOGLE_SAT_MAP,
                            QGeoMapType::SatelliteMapDay, parent)
    {
    }

protected:
    QString getURL(const int x, const int y, const int zoom,
                   QNetworkAccessManager* networkManager) override;
};

// -----------------------------------------------------------
// Google Labels Map

class TiGoogleLabelsMapProvider : public TiGoogleMapProvider
{
    Q_OBJECT

public:
    TiGoogleLabelsMapProvider(QObject* parent = nullptr) :
        TiGoogleMapProvider(QStringLiteral("png"), AVERAGE_TILE_SIZE, QGeoMapType::CustomMap, parent)
    {
    }

protected:
    QString getURL(const int x, const int y, const int zoom,
                   QNetworkAccessManager* networkManager) override;
};

// -----------------------------------------------------------
// Google Terrain Map

class TiGoogleTerrainMapProvider : public TiGoogleMapProvider
{
    Q_OBJECT

public:
    TiGoogleTerrainMapProvider(QObject* parent = nullptr) :
        TiGoogleMapProvider(QStringLiteral("png"), AVERAGE_GOOGLE_TERRAIN_MAP,
                            QGeoMapType::TerrainMap, parent)
    {
    }

protected:
    QString getURL(const int x, const int y, const int zoom,
                   QNetworkAccessManager* networkManager) override;
};

// -----------------------------------------------------------
// Google Hybrid Map

class TiGoogleHybridMapProvider : public TiGoogleMapProvider
{
    Q_OBJECT

public:
    TiGoogleHybridMapProvider(QObject* parent = nullptr) :
        TiGoogleMapProvider(QStringLiteral("png"), AVERAGE_GOOGLE_SAT_MAP, QGeoMapType::HybridMap, parent)
    {
    }

protected:
    QString getURL(const int x, const int y, const int zoom,
                   QNetworkAccessManager* networkManager) override;
};
