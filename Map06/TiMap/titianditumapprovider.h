/**************************************************************************
 *   文件名	：titianditumapprovider.h
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
#pragma once
#include "timapprovider.h"

class TiTiandituMapProvider : public TiMapProvider
{
public:
    TiTiandituMapProvider(const QString& format, const quint32 averageSize,
                          const QGeoMapType::MapStyle mapType = QGeoMapType::CustomMap,
                          QObject*                    parent  = nullptr);

    // TiMapProvider interface
public:
    void setToken(const QString& token) override;

protected:
    QString m_token;
};

/**
 * @brief The TiTiandituImgMapProvider class 天地图 卫星影像图
 */
class TiTiandituImgMapProvider : public TiTiandituMapProvider
{
public:
    TiTiandituImgMapProvider(QObject* parent = nullptr);
    ~TiTiandituImgMapProvider();

protected:
    QString getURL(const int x, const int y, const int zoom,
                   QNetworkAccessManager* networkManager) override;
};

/**
 * @brief The TiTiandituCiaMapProvider class 天地图 影像图标注
 */
class TiTiandituCiaMapProvider : public TiTiandituMapProvider
{
public:
    TiTiandituCiaMapProvider(QObject* parent = nullptr);
    ~TiTiandituCiaMapProvider();

protected:
    QString getURL(const int x, const int y, const int zoom,
                   QNetworkAccessManager* networkManager) override;
};

///
/// \brief The TiTiandituVecMapProvider class 天地图失量底图
///
class TiTiandituVecMapProvider : public TiTiandituMapProvider
{
public:
    TiTiandituVecMapProvider(QObject* parent = nullptr);
    ~TiTiandituVecMapProvider();

protected:
    QString getURL(const int              x,
                   const int              y,
                   const int              zoom,
                   QNetworkAccessManager* networkManager) override;
};
class TiTiandituCvaMapProvider : public TiTiandituMapProvider
{
public:
    TiTiandituCvaMapProvider(QObject* parent = nullptr);
    ~TiTiandituCvaMapProvider();

protected:
    QString getURL(const int              x,
                   const int              y,
                   const int              zoom,
                   QNetworkAccessManager* networkManager) override;
};
