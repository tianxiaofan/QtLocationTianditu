/**************************************************************************
 *   文件名	：tigeotilefetcher.cpp
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
#include "tigeotilefetcher.h"
#include "tiqgeotiledmapreply.h"
#include "timapprovider.h"
#include "tilogger.h"
#include "timapengine.h"

TiGeoTileFetcher::TiGeoTileFetcher(const QVariantMap& parameters, QGeoMappingManagerEngine* parent) :
    QGeoTileFetcher(parent), m_networkManager(new QNetworkAccessManager(this))
{
}

//重写QGeoTileFetcher 的虚函数,由QGeoTileFetcher 调用
QGeoTiledMapReply* TiGeoTileFetcher::getTileImage(const QGeoTileSpec& spec)
{
    QNetworkRequest request = TiMapEngine::instance()->getUrlEngine()->getTileUrl(
            spec.mapId(), spec.x(), spec.y(), spec.zoom(), m_networkManager);
    return new TiQGeoTiledMapReply(m_networkManager, request, spec);
}
