/**************************************************************************
 *   文件名	：timapprovider.cpp
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
#include "timapprovider.h"

// png,jpg,gif识别
static const unsigned char pngSignature[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00 };
static const unsigned char jpegSignature[] = { 0xFF, 0xD8, 0xFF, 0x00 };
static const unsigned char gifSignature[]  = { 0x47, 0x49, 0x46, 0x38, 0x00 };

TiMapProvider::TiMapProvider(const QString& imageFormat, const quint32 averageSize,
                             const QGeoMapType::MapStyle mapType, QObject* parent) :
    QObject(parent), m_mapType(mapType), m_imageFormat(imageFormat), m_averageSize(averageSize)
{
    m_language = QStringLiteral("zh-CN");
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
        QByteArrayLiteral("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.45 Safari/537.36"));
    request.setUrl(QUrl(url));
    return request;
}

QString TiMapProvider::getImageFormat(const QByteArray& image) const
{
    QString format;
    if (image.size() > 2) {
        if (image.startsWith(reinterpret_cast<const char*>(pngSignature)))
            format = QStringLiteral("png");
        else if (image.startsWith(reinterpret_cast<const char*>(jpegSignature)))
            format = QStringLiteral("jpg");
        else if (image.startsWith(reinterpret_cast<const char*>(gifSignature)))
            format = QStringLiteral("gif");
        else {
            return m_imageFormat;
        }
    }
    return format;
}

QString TiMapProvider::tileXYToQuadKey(const int tileX, const int tileY, const int levelOfDetail) const
{
    QString quadKey;
    for (int i = levelOfDetail; i > 0; i--) {
        char      digit = '0';
        const int mask  = 1 << (i - 1);
        if ((tileX & mask) != 0) {
            digit++;
        }
        if ((tileY & mask) != 0) {
            digit++;
            digit++;
        }
        quadKey.append(digit);
    }
    return quadKey;
}

int TiMapProvider::getServerNum(const int x, const int y, const int max) const
{
    return (x + 2 * y) % max;
}
