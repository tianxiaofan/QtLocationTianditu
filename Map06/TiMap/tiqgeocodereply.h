/**************************************************************************
 *   文件名	：tiqgeocodereply.h
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
#include <QGeoCodeReply>
#include <QNetworkReply>

class TiQGeoCodeReply : public QGeoCodeReply
{
    Q_OBJECT
public:
    explicit TiQGeoCodeReply(QNetworkReply* reply, QObject* parent = nullptr);
    ~TiQGeoCodeReply();

private slots:
    void networkReplyFinish();
    void networkReplyError(QNetworkReply::NetworkError error);

private:
    QNetworkReply* m_reply;
};

