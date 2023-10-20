/**************************************************************************
 *   文件名	：tiqgeocodingmanagerengine.cpp
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-5-17
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
#include "tiqgeocodingmanagerengine.h"
#include "tiqgeocodereply.h"
#include "tilogger.h"

#include <QGeoAddress>
#include <QGeoRectangle>
#include <QGeoShape>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>

static QString addressToQuery(const QGeoAddress& address)
{
    return address.street() + QStringLiteral(", ") + address.district() + QStringLiteral(", ")
           + address.city() + QStringLiteral(", ") + address.state() + QStringLiteral(", ")
           + address.country();
}

static QString boundingBoxToLtrb(const QGeoRectangle& rect)
{
    return QString::number(rect.topLeft().longitude()) + QLatin1Char(',')
           + QString::number(rect.topLeft().latitude()) + QLatin1Char(',')
           + QString::number(rect.bottomRight().longitude()) + QLatin1Char(',')
           + QString::number(rect.bottomRight().latitude());
}

const QString user_agent = QStringLiteral("useragent");

TiQGeoCodingManagerEngine::TiQGeoCodingManagerEngine(const QVariantMap&          parameters,
                                                     QGeoServiceProvider::Error* error,
                                                     QString*                    errorString) :
    QGeoCodingManagerEngine(parameters), m_networkManager(new QNetworkAccessManager(this))
{
    LOG_DEBUG() << parameters;
    if (parameters.contains(user_agent))
        m_userAgent = parameters.value(user_agent).toString().toLatin1();
    else
        m_userAgent = "Qt Location based application";
    *error = QGeoServiceProvider::NoError;
    errorString->clear();
}

QGeoCodeReply* TiQGeoCodingManagerEngine::geocode(const QGeoAddress& address, const QGeoShape& bounds)
{
    return geocode(addressToQuery(address), -1, -1, bounds);
}

QGeoCodeReply* TiQGeoCodingManagerEngine::geocode(const QString& address, int limit, int offset,
                                                  const QGeoShape& bounds)
{
    Q_UNUSED(limit)
    Q_UNUSED(offset)

    QNetworkRequest request;
    request.setRawHeader("User-Agent", m_userAgent);

    QUrl      url(QStringLiteral("http://maps.googleapis.com/maps/api/geocode/json"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("sensor"), QStringLiteral("false"));
    query.addQueryItem(QStringLiteral("language"), locale().name().left(2));
    query.addQueryItem(QStringLiteral("address"), address);
    if (bounds.type() == QGeoShape::RectangleType)
        query.addQueryItem(QStringLiteral("bounds"), boundingBoxToLtrb(bounds));

    url.setQuery(query);

    request.setUrl(url);

    QNetworkReply* reply = m_networkManager->get(request);
    //这里不设置父类,让其自己在接收数据结束后自己delete自己
    reply->setParent(nullptr);

    TiQGeoCodeReply* geocodeReply = new TiQGeoCodeReply(reply);
    connect(geocodeReply, &TiQGeoCodeReply::finished, this, &TiQGeoCodingManagerEngine::replyFinished);
    //无法使用connect进行函数地址连接,,请查看官方文档
    connect(geocodeReply, QOverload<QGeoCodeReply::Error, const QString&>::of(&QGeoCodeReply::error),
            this, [=](QGeoCodeReply::Error error, const QString& errorString) {
                replyError(error, errorString);
            });
    return geocodeReply;
}

QGeoCodeReply* TiQGeoCodingManagerEngine::reverseGeocode(const QGeoCoordinate& coordinate,
                                                         const QGeoShape&      bounds)
{
    QNetworkRequest request;
    request.setRawHeader("User-Agent", m_userAgent);

    QUrl      url(QStringLiteral("http://maps.googleapis.com/maps/api/geocode/json"));
    QUrlQuery query;

    query.addQueryItem(QStringLiteral("sensor"), QStringLiteral("false"));
    query.addQueryItem(QStringLiteral("language"), locale().name().left(2));
    query.addQueryItem(QStringLiteral("latlng"),
                       QStringLiteral("%1,%2").arg(coordinate.latitude(), coordinate.longitude()));
    url.setQuery(query);
    request.setUrl(url);

    QNetworkReply* reply = m_networkManager->get(request);
    reply->setParent(nullptr);

    TiQGeoCodeReply* geocodeReply = new TiQGeoCodeReply(reply);
    connect(geocodeReply, &TiQGeoCodeReply::finished, this, &TiQGeoCodingManagerEngine::replyFinished);
    //无法使用connect进行函数地址连接,,请查看官方文档
    connect(geocodeReply, QOverload<QGeoCodeReply::Error, const QString&>::of(&QGeoCodeReply::error),
            this, [=](QGeoCodeReply::Error error, const QString& errorString) {
                replyError(error, errorString);
            });
    return geocodeReply;
}

void TiQGeoCodingManagerEngine::replyFinished()
{
    QGeoCodeReply* reply = qobject_cast<QGeoCodeReply*>(sender());
    if (reply)
        emit finished(reply);
}

void TiQGeoCodingManagerEngine::replyError(QGeoCodeReply::Error errorCode, const QString& errorString)
{
    QGeoCodeReply* reply = qobject_cast<QGeoCodeReply*>(sender());
    if (reply)
        emit error(reply, errorCode, errorString);
}
