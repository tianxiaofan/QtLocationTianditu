/**************************************************************************
 *   文件名	：tigeotilefetcher.cpp
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
#include "tigeotilefetcher.h"
#include "tiqgeotiledmapreply.h"
#include "timapprovider.h"
#include <QDebug>
#include "timapengine.h"

TiGeoTileFetcher::TiGeoTileFetcher(const QVariantMap& parameters, QGeoMappingManagerEngine* parent) :
    QGeoTileFetcher(parent), m_networkManager(new QNetworkAccessManager(this))
{
    QString _provider = "mapProvider", _format = "format";
    //链接地址
    if (parameters.contains(_provider)) {
        m_provider = parameters.value(_provider).toString();
        qDebug() << m_provider;
        if (m_provider == QString("tiandituImg"))
            m_urlProvider = "img";
        else if (m_provider == QString("tiandituCia"))
            m_urlProvider = "cia";
    }

    //瓦片图片格式
    if (parameters.contains(_format)) {
        m_format = parameters.value(_format).toString();
        qDebug() << m_format;
    }
}

//重写QGeoTileFetcher 的虚函数,由QGeoTileFetcher 调用
QGeoTiledMapReply* TiGeoTileFetcher::getTileImage(const QGeoTileSpec& spec)
{
    QNetworkRequest request = TiMapEngine::instance()->getUrlEngine()->getTileUrl(spec.mapId(),
                                                                                  spec.x(),
                                                                                  spec.y(),
                                                                                  spec.zoom(),
                                                                                  m_networkManager);
    //    qDebug() << request.url() << request.rawHeaderList();
    QNetworkReply* reply = m_networkManager->get(request);

    return new TiQGeoTiledMapReply(reply, spec, m_format);
}
