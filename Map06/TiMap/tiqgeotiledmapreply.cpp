/**************************************************************************
 *   文件名	：tiqgeotiledmapreply.cpp
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
#include "tiqgeotiledmapreply.h"
#include "tilogger.h"
#include "timapengine.h"
#include "timapprovider.h"

#include <private/qgeotilespec_p.h>

TiQGeoTiledMapReply::TiQGeoTiledMapReply(QNetworkAccessManager* networkManger,
                                         const QNetworkRequest& request, const QGeoTileSpec& spec,
                                         QObject* parent) :
    QGeoTiledMapReply(spec, parent), m_reply(nullptr), m_request(request), m_networkManager(networkManger)
{
    //如果请求地址为空,显示空白tile
    if (m_request.url().isEmpty()) {
        if (!m_noTitle.size()) {
            QFile fi(":/resources/TiMapImage/notile.png");
            if (fi.open(QFile::ReadOnly))
                m_noTitle = fi.readAll();
        }
        setMapImageData(m_noTitle);
        setMapImageFormat("png");
        setFinished(true);
        setCached(false);
    } else {
        //如果地址不为空,从本地数据库获取tile
        auto task = TiMapEngine::instance()->createFetchTileTask(spec.mapId(), spec.x(), spec.y(),
                                                                 spec.zoom());
        connect(task, &TiMapFetchTileTask::tileFetched, this, &TiQGeoTiledMapReply::onCacheReply);
        connect(task, &TiMapFetchTileTask::error, this, &TiQGeoTiledMapReply::onCacheError);
        TiMapEngine::instance()->addTask(task);
    }
}

TiQGeoTiledMapReply::~TiQGeoTiledMapReply()
{
    closeReply();
}

void TiQGeoTiledMapReply::abort()
{
    m_timer.stop();
    if (m_reply)
        m_reply->abort();
    emit aborted();
}

/**
 * @brief TiQGeoTiledMapReply::networkReplyFinished 从网络中获取到tile
 */
void TiQGeoTiledMapReply::networkReplyFinished()
{
    //定时器还在运行,停止
    m_timer.stop();

    //可能已是获取超时
    if (!m_reply) {
        LOG_DEBUG() << "reply null:" << hash();
        emit aborted();
        return;
    }
    //获取错误
    if (m_reply->error() != QNetworkReply::NoError) {
        LOG_DEBUG() << "reply error:" << hash();
        emit aborted();
        return;
    }

    //获取到tile
    auto    img    = m_reply->readAll();
    QString format = TiMapEngine::instance()->getUrlEngine()->getImageFormat(tileSpec().mapId(), img);
    //判断是否是高程服务
    auto isElv = TiMapEngine::instance()->getUrlEngine()->isElevation(tileSpec().mapId());
    if (isElv) {
        LOG_DEBUG() << "isElv:" << hash();

    } else {
        //非高程数据
        auto mapProvider
                = TiMapEngine::instance()->getUrlEngine()->getMapProviderFromId(tileSpec().mapId());
        //可放大的图层
        if (mapProvider && mapProvider->isBingProvider() && img.size()) {
            setError(QGeoTiledMapReply::CommunicationError, "Bing tile above zoom level");
            LOG_DEBUG() << "bing :" << hash();
        } else {
            //正常图层
            setMapImageData(img);
            if (!format.isEmpty()) {
                setMapImageFormat(format);
                //                LOG_DEBUG() << "write database:" << hash() << (TiMapEngine::instance()->cacheCount);
                //缓冲到数据库
                TiMapEngine::instance()->cacheTile(tileSpec().mapId(), tileSpec().x(),
                                                   tileSpec().y(), tileSpec().zoom(), img, format);
            } else {
                LOG_DEBUG() << "format null:" << hash();
            }
        }
        setFinished(true);
    }
    closeReply();
}

void TiQGeoTiledMapReply::networkReplyError(QNetworkReply::NetworkError error)
{
    m_timer.stop();

    LOG_DEBUG() << "reply error:" << hash();
    if (!m_reply) {
        return;
    }

    LOG_DEBUG() << "reply error:" << error << hash();
    setError(QGeoTiledMapReply::CommunicationError, m_reply->errorString());
    TiMapEngine::instance()->testInternet();
    setFinished(true);
    closeReply();
}

/**
 * @brief TiQGeoTiledMapReply::onCacheReply 正常从数据库中获取到tile
 * @param tile
 */
void TiQGeoTiledMapReply::onCacheReply(TiMapTile *tile)
{
    if (tile->x() == tileSpec().x() && tile->y() == tileSpec().y()
        && tile->zoom() == tileSpec().zoom()) {
        setMapImageFormat(tile->format());
        setMapImageData(tile->img());
        setFinished(true);
        setCached(true);
    }

    tile->deleteLater();
}

/**
 * @brief TiQGeoTiledMapReply::onCacheError 从数据库中获取tile error
 * @param type
 * @param errorString
 */
void TiQGeoTiledMapReply::onCacheError(TiMapTask::TaskType type, QString errorString)
{
    //无网络
    if (!TiMapEngine::instance()->internetActive()) {
        TiMapEngine::instance()->testInternet();
        LOG_DEBUG() << "no internet:" << hash();
        setFinished(true);
    } else { //网络正常
        m_reply = m_networkManager->get(m_request);

        m_reply->setParent(nullptr);
        //接收结束后,读取接收到的数据
        connect(m_reply, &QNetworkReply::finished, this, &TiQGeoTiledMapReply::networkReplyFinished);
        //发生错误时,释放reply
        connect(m_reply, &QNetworkReply::errorOccurred, this, &TiQGeoTiledMapReply::networkReplyError);

        //防止m_reply不会释放造成内存泄露,改为由析构函数释放
        // connect(this, &TiQGeoTiledMapReply::destroyed, m_reply, &QNetworkReply::deleteLater);

        //等待10秒的时间获取tile,超时后结束获取
        connect(&m_timer, &QTimer::timeout, this, &TiQGeoTiledMapReply::onTimeout);
        m_timer.setSingleShot(true);
        m_timer.start(10000);
    }
}

void TiQGeoTiledMapReply::onTimeout()
{
    closeReply();
    emit aborted();
    LOG_DEBUG() << "timeout:" << hash();
}

void TiQGeoTiledMapReply::closeReply()
{
    m_timer.stop();
    if (m_reply) {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}

quint32 TiQGeoTiledMapReply::hash()
{
    return TiMapEngine::instance()->getTileHash(tileSpec().mapId(), tileSpec().x(), tileSpec().y(),
                                                tileSpec().zoom());
}
