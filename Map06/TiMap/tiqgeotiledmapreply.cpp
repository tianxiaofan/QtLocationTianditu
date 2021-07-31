/**************************************************************************
 *   文件名	：tiqgeotiledmapreply.cpp
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-5-12
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
#include "tiqgeotiledmapreply.h"
#include <QDebug>

TiQGeoTiledMapReply::TiQGeoTiledMapReply(QNetworkReply* reply, const QGeoTileSpec& spec,
                                         const QString& imageFormat, QObject* parent) :
    QGeoTiledMapReply(spec, parent)
{
    if (!reply) {
        setError(UnknownError, QStringLiteral("null reply"));
        return;
    }
    //接收结束后,读取接收到的数据
    connect(reply, &QNetworkReply::finished, this, &TiQGeoTiledMapReply::networkReplyFinished);
    //发生错误时,释放reply
    connect(reply, &QNetworkReply::errorOccurred, this, &TiQGeoTiledMapReply::networkReplyError);
    connect(this, &TiQGeoTiledMapReply::aborted, reply, &QNetworkReply::abort);
    connect(this, &TiQGeoTiledMapReply::destroyed, reply, &QNetworkReply::deleteLater);
    setMapImageFormat(imageFormat);
}

void TiQGeoTiledMapReply::networkReplyFinished()
{
    QNetworkReply* reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError)
        return;

    setMapImageData(reply->readAll());
    setFinished(true);
}

void TiQGeoTiledMapReply::networkReplyError(QNetworkReply::NetworkError error)
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    reply->deleteLater();
    if (error == QNetworkReply::OperationCanceledError)
        setFinished(true);
    else
        setError(QGeoTiledMapReply::CommunicationError, reply->errorString());
}
