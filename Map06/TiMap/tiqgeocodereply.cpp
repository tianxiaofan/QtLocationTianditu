/**************************************************************************
 *   文件名	：tiqgeocodereply.cpp
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
#include "tiqgeocodereply.h"
#include "tilogger.h"

#include <QJsonDocument>
#include <QJsonObject>

TiQGeoCodeReply::TiQGeoCodeReply(QNetworkReply* reply, QObject* parent) :
    QGeoCodeReply(parent), m_reply(reply)
{
    connect(m_reply, &QNetworkReply::finished, this, &TiQGeoCodeReply::networkReplyFinish);
    connect(m_reply, &QNetworkReply::errorOccurred, this, &TiQGeoCodeReply::networkReplyError);

    //将来自每个数据源的响应数量的限制设置为限制。
    setLimit(1);
    //设置整个结果集中的偏移量，从该偏移量开始将结果取到偏移量。
    setOffset(0);
}

TiQGeoCodeReply::~TiQGeoCodeReply()
{
    if (m_reply)
        m_reply->deleteLater();
}

void TiQGeoCodeReply::networkReplyFinish()
{
    if (!m_reply)
        return;

    if (m_reply->error() != QNetworkReply::NoError)
        return;

    QJsonDocument document = QJsonDocument::fromJson(m_reply->readAll());
    QJsonObject   object   = document.object();

    const QString sta = QStringLiteral("status");
    if (object.value(sta) != QStringLiteral("OK")) {
        QString error = object.value(sta).toString();

        LOG_DEBUG() << m_reply->url() << "returned :" << error;

        setError(QGeoCodeReply::CommunicationError, error);
        m_reply->deleteLater();
        m_reply = nullptr;
        return;
    }
}

void TiQGeoCodeReply::networkReplyError(QNetworkReply::NetworkError error) { }
