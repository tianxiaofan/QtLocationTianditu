/**************************************************************************
 *   文件名	：timapengine.cpp
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-6-16
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
#include "timapengine.h"
#include "tilogger.h"

#include <QDir>
#include <QStandardPaths>

static const char* dbName = "TiMapCache.db";
TiMapEngine::TiMapEngine(QObject* parent) : QObject(parent)
{
    m_internetState = false;
    cacheCount      = 0;
    LOG_DEBUG() << "instance !";
    qRegisterMetaType<TiMapTask::TaskType>();
    init();

    connect(&m_timer, &QTimer::timeout, this, &TiMapEngine::onTimeout);
    testInternet();

    m_mapUrlEngine = new TiMapUrlEngine(this);
}

TiMapEngine::~TiMapEngine()
{
    m_worker.quit();
    m_worker.wait();
}

/**
 * @brief TiMapEngine::createFetchTileTask 创建tile获取任务
 * @param mapId
 * @param x
 * @param y
 * @param zoom
 * @return
 */
TiMapFetchTileTask* TiMapEngine::createFetchTileTask(int mapId, int x, int y, int zoom)
{
    auto hash = getTileHash(mapId, x, y, zoom);
    auto task = new TiMapFetchTileTask(hash);
    return task;
}

/**
 * @brief TiMapEngine::testInternet 测试网络连接,网络不通时会以5s周期一直测试
 */
void TiMapEngine::testInternet()
{
    if (m_timer.isActive())
        return;
    onTimeout();
}

void TiMapEngine::addTask(TiMapTask* task)
{
    m_worker.enqueueTask(task);
}

void TiMapEngine::cacheTile(const QString& type, int x, int y, int zoom, const QByteArray& img,
                            const QString& format)
{
    if (isCache()) {
        auto mapid = getUrlEngine()->getIdFromType(type);
        addTile(type, mapid, x, y, zoom, img, format);
    }
}

void TiMapEngine::cacheTile(int mapId, int x, int y, int zoom, const QByteArray& img,
                            const QString& format)
{
    if (isCache()) {
        auto type = getUrlEngine()->getTypeFromId(mapId);
        addTile(type, mapId, x, y, zoom, img, format);
    }
}

/**
 * @brief TiMapEngine::getTileHash 获取一个tile唯一的hash
 * @param mapId
 * @param x
 * @param y
 * @param zoom
 * @return
 */
quint32 TiMapEngine::getTileHash(int mapId, int x, int y, int zoom)
{
    auto str = QString::asprintf("%010d%08d%08d%03d", mapId, x, y, zoom);
    return qHash(str);
}

void TiMapEngine::init()
{
    //初始化设置worker数据库路径
    m_dbPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
               + QStringLiteral("/TiMap");
    QDir di;
    if (!di.exists(m_dbPath))
        di.mkpath(m_dbPath);
    m_dbName = dbName;
    if (!m_dbPath.isEmpty() && !m_dbName.isEmpty()) {
        m_worker.setDBName(m_dbPath + "/" + m_dbName);
    }
}

void TiMapEngine::addTile(const QString& type, int mapid, int x, int y, int zoom,
                          const QByteArray& img, const QString& format)
{

    auto tile = new TiMapTile(getTileHash(mapid, x, y, zoom));
    tile->setFormat(format);
    tile->setImg(img);
    tile->setMapId(mapid);
    tile->setType(type);
    tile->setX(x);
    tile->setY(y);
    tile->setZoom(zoom);
    auto task = new TiMapSaveTileTask(tile);
    m_worker.enqueueTask(task);
}

void TiMapEngine::onTimeout()
{
    m_timer.start(5000);
    QHostInfo::lookupHost("www.baidu.com", this, [&](QHostInfo info) {
        if (info.error() == QHostInfo::NoError) {
            m_internetState = true;
            LOG_INFO() << "internet online";
            m_timer.stop();
        } else {
            m_internetState = false;
            LOG_INFO() << "internet offline";
        }
    });
}
