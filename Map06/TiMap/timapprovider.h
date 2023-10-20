/**************************************************************************
 *   文件名	：timapprovider.h
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
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QObject>

#include <private/qgeomaptype_p.h>

class TiMapProvider : public QObject
{
    Q_OBJECT
public:
    TiMapProvider(const QString& imageFormat, const quint32 averageSize,
                  const QGeoMapType::MapStyle mapType = QGeoMapType::CustomMap,
                  QObject*                    parent  = nullptr);

    /**
     * @brief getTileURL
     * @return 返回值有可能为空,在使用时需进行判断
     */
    virtual QNetworkRequest getTileURL(const int x, const int y, const int zoom,
                                       QNetworkAccessManager* networkManager);
    virtual void            setToken(const QString& token) { Q_UNUSED(token) }
    virtual bool            isElevationProvider() const { return false; }
    virtual bool            isBingProvider() const { return false; }

    QGeoMapType::MapStyle getMapStyle() const { return m_mapType; }
    QString               getImageFormat(const QByteArray& image) const;

    enum { AVERAGE_TILE_SIZE = 13652 };

protected:
    virtual QString getURL(const int x, const int y, const int zoom, QNetworkAccessManager* networkManager)
            = 0;

    QString tileXYToQuadKey(const int tileX, const int tileY, const int levelOfDetail) const;
    int     getServerNum(const int x, const int y, const int max) const;

protected:
    QGeoMapType::MapStyle m_mapType;
    QString               m_referrer;
    QString               m_imageFormat;
    quint32               m_averageSize;
    QByteArray            m_userAgent;
    QString               m_language;
};
