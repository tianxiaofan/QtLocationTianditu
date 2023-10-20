/**************************************************************************
 *   文件名	：titianditumapprovider.cpp
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-6-16
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
#include "titianditumapprovider.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "timapengine.h"
#include "tilogger.h"

//天地图密钥
static const char* token = "a8341d81e4fd52009981dffe1e77943a";

TiTiandituMapProvider::TiTiandituMapProvider(const QString& format, const quint32 averageSize,
                                             const QGeoMapType::MapStyle mapType, QObject* parent) :
    TiMapProvider(format, averageSize, mapType, parent)
{
}

void TiTiandituMapProvider::setToken(const QString& token)
{
    if (token.isEmpty() && token == m_token)
        return;
    m_token = token;
}

static const QString tiandituImg
        = QStringLiteral("http://t3.tianditu.gov.cn/img_w/"
                         "wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=img&STYLE=default&"
                         "TILEMATRIXSET=w&FORMAT=tiles&TILEMATRIX=%1&TILEROW=%2&TILECOL=%3&tk=%4");
TiTiandituImgMapProvider::TiTiandituImgMapProvider(QObject* parent) :
    TiTiandituMapProvider("png", AVERAGE_TILE_SIZE, QGeoMapType::SatelliteMapDay, parent)
{
    setToken(token);
}

TiTiandituImgMapProvider::~TiTiandituImgMapProvider() { }

QString TiTiandituImgMapProvider::getURL(const int x, const int y, const int zoom,
                                         QNetworkAccessManager* networkManager)
{
    Q_UNUSED(networkManager)
    const QString xx = QString("%1").arg(x);
    const QString yy = QString("%1").arg(y);
    const QString zz = QString("%1").arg(zoom);
    return tiandituImg.arg(zz, yy, xx, m_token);
}

static const QString tiandituCia
        = QStringLiteral("http://t3.tianditu.gov.cn/cia_w/"
                         "wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=cia&STYLE=default&"
                         "TILEMATRIXSET=w&FORMAT=tiles&TILEMATRIX=%1&TILEROW=%2&TILECOL=%3&tk=%4");
TiTiandituCiaMapProvider::TiTiandituCiaMapProvider(QObject* parent) :
    TiTiandituMapProvider("png", AVERAGE_TILE_SIZE, QGeoMapType::StreetMap, parent)
{
    setToken(token);
}

TiTiandituCiaMapProvider::~TiTiandituCiaMapProvider() { }

QString TiTiandituCiaMapProvider::getURL(const int x, const int y, const int zoom,
                                         QNetworkAccessManager* networkManager)
{
    Q_UNUSED(networkManager)
    const QString xx = QString("%1").arg(x);
    const QString yy = QString("%1").arg(y);
    const QString zz = QString("%1").arg(zoom);
    return tiandituCia.arg(zz, yy, xx, m_token);
}

static const QString tiandituVec = QStringLiteral(
    "http://t3.tianditu.gov.cn/vec_w/"
    "wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=vec&STYLE=default&"
    "TILEMATRIXSET=w&FORMAT=tiles&TILEMATRIX=%1&TILEROW=%2&TILECOL=%3&tk=%4");

TiTiandituVecMapProvider::TiTiandituVecMapProvider(QObject* parent)
    : TiTiandituMapProvider("png", AVERAGE_TILE_SIZE, QGeoMapType::StreetMap, parent)
{
    setToken(token);
}

TiTiandituVecMapProvider::~TiTiandituVecMapProvider() {}

QString TiTiandituVecMapProvider::getURL(const int              x,
                                         const int              y,
                                         const int              zoom,
                                         QNetworkAccessManager* networkManager)
{
    Q_UNUSED(networkManager)
    const QString xx = QString("%1").arg(x);
    const QString yy = QString("%1").arg(y);
    const QString zz = QString("%1").arg(zoom);
    return tiandituVec.arg(zz, yy, xx, m_token);
}

static const QString tiandituCva = QStringLiteral(
    "http://t3.tianditu.gov.cn/cva_w/"
    "wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=cva&STYLE=default&"
    "TILEMATRIXSET=w&FORMAT=tiles&TILEMATRIX=%1&TILEROW=%2&TILECOL=%3&tk=%4");
TiTiandituCvaMapProvider::TiTiandituCvaMapProvider(QObject* parent)
    : TiTiandituMapProvider("png", AVERAGE_TILE_SIZE, QGeoMapType::StreetMap, parent)
{
    setToken(token);
}

TiTiandituCvaMapProvider::~TiTiandituCvaMapProvider() {}

QString TiTiandituCvaMapProvider::getURL(const int              x,
                                         const int              y,
                                         const int              zoom,
                                         QNetworkAccessManager* networkManager)
{
    Q_UNUSED(networkManager)
    const QString xx = QString("%1").arg(x);
    const QString yy = QString("%1").arg(y);
    const QString zz = QString("%1").arg(zoom);
    return tiandituCva.arg(zz, yy, xx, m_token);
}
