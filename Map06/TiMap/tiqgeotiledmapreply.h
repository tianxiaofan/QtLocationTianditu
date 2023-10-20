/**************************************************************************
 *   文件名	：tiqgeotiledmapreply.h
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-5-12
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
#include <private/qgeotiledmapreply_p.h>
#include <QNetworkReply>
#include <QTimer>

#include "timapengine.h"

class TiQGeoTiledMapReply : public QGeoTiledMapReply
{
    Q_OBJECT
public:
    TiQGeoTiledMapReply(QNetworkAccessManager* networkManger, const QNetworkRequest& reply,
                        const QGeoTileSpec& spec, QObject* parent = nullptr);
    ~TiQGeoTiledMapReply();
    void abort() override;

private slots:
    void networkReplyFinished();
    void networkReplyError(QNetworkReply::NetworkError error);

    void onCacheReply(TiMapTile* tile);
    void onCacheError(TiMapTask::TaskType type, QString errorString);

    void onTimeout();

private:
    void    closeReply();
    quint32 hash();

private:
    QNetworkReply*         m_reply;
    QNetworkRequest        m_request;
    QNetworkAccessManager* m_networkManager;
    QByteArray             m_noTitle;
    QTimer                 m_timer;
};

