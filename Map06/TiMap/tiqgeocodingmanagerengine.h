/**************************************************************************
 *   文件名	：tiqgeocodingmanagerengine.h
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

#pragma once
#include <QGeoCodingManagerEngine>
#include <QGeoServiceProvider>
class QNetworkAccessManager;
class TiQGeoCodingManagerEngine : public QGeoCodingManagerEngine
{
    Q_OBJECT
public:
    TiQGeoCodingManagerEngine(const QVariantMap& parameters, QGeoServiceProvider::Error* error,
                              QString* errorString);

    // QGeoCodingManagerEngine interface
public:
    QGeoCodeReply* geocode(const QGeoAddress& address, const QGeoShape& bounds) override;
    QGeoCodeReply* geocode(const QString& address, int limit, int offset,
                           const QGeoShape& bounds) override;
    QGeoCodeReply* reverseGeocode(const QGeoCoordinate& coordinate, const QGeoShape& bounds) override;

private slots:
    // replay 结束
    void replyFinished();
    // replay 错误
    void replyError(QGeoCodeReply::Error errorCode, const QString& errorString);

private:
    QNetworkAccessManager* m_networkManager;
    QByteArray             m_userAgent;
};
