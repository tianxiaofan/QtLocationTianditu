/**************************************************************************
 *   文件名	：tigeotilefetcher.h
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
#include <private/qgeotilefetcher_p.h>

#include <QNetworkAccessManager>

class TiGeoTileFetcher : public QGeoTileFetcher
{
public:
    TiGeoTileFetcher(const QVariantMap& parameters, QGeoMappingManagerEngine* parent);

    // QGeoTileFetcher interface
private:
    QGeoTiledMapReply* getTileImage(const QGeoTileSpec& spec) override;

private:
    QString                m_provider;    //服务商类型
    QString                m_urlProvider; //天地图url中用于区分图源类型
    QString                m_format         = "png";
    QNetworkAccessManager* m_networkManager = nullptr;

};
