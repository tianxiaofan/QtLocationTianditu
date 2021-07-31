/**************************************************************************
 *   文件名	：timapprovider.h
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-6-16
 *   邮   箱	：499131808@qq.com
 *   Q Q		：499131808
 *   公   司      
 *   功能描述      ：
 *   使用说明 ：
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
    TiMapProvider(const QGeoMapType::MapStyle mapType = QGeoMapType::CustomMap,
                  QObject*                    parent  = nullptr);

    /**
     * @brief getTileURL
     * @return 返回值有可能为空,在使用时需进行判断
     */
    virtual QNetworkRequest getTileURL(const int x, const int y, const int zoom,
                                       QNetworkAccessManager* networkManager);
    virtual void            setToken(const QString& token) { Q_UNUSED(token) }

    QGeoMapType::MapStyle getMapStyle() const { return m_mapType; }

protected:
    virtual QString getURL(const int x, const int y, const int zoom, QNetworkAccessManager* networkManager)
            = 0;

protected:
    QGeoMapType::MapStyle m_mapType;
};
