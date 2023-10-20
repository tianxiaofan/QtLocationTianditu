/**************************************************************************
 *   文件名	：timapprovider.cpp
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
#include "timapprovider.h"

TiMapProvider::TiMapProvider(const QGeoMapType::MapStyle mapType, QObject* parent) :
    QObject(parent), m_mapType(mapType)
{
}

QNetworkRequest TiMapProvider::getTileURL(const int x, const int y, const int zoom,
                                          QNetworkAccessManager* networkManager)
{
    QNetworkRequest request;
    auto            url = getURL(x, y, zoom, networkManager);
    if (url.isEmpty())
        return request;

    request.setRawHeader(QByteArrayLiteral("Accept"), QByteArrayLiteral("text/html,image/*"));
    request.setRawHeader(QByteArrayLiteral("User-Agent"),
                         QByteArrayLiteral(
                             "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, "
                             "like Gecko) Chrome/96.0.4664.45 Safari/537.36"));
    request.setUrl(QUrl(url));
    return request;
}
