/**************************************************************************
 *   文件名	：tiqgeotiledmapreply.h
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

#pragma once
#include <private/qgeotiledmapreply_p.h>
#include <QNetworkReply>

class TiQGeoTiledMapReply : public QGeoTiledMapReply
{
    Q_OBJECT
public:
    TiQGeoTiledMapReply(QNetworkReply* reply, const QGeoTileSpec& spec, const QString& imageFormat,
                        QObject* parent = nullptr);

private slots:
    void networkReplyFinished();
    void networkReplyError(QNetworkReply::NetworkError error);
};

