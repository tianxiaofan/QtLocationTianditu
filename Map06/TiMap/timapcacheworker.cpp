/**************************************************************************
 *   文件名	：TiMapCacheWorker.cpp
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-5-14
 *   邮   箱	：499131808@qq.com
 *   Q Q		：499131808
 *   公   司      ：
 *   功能描述      ：
 *   使用说明
 *：任何人或机构要使用、修改、发布本源代码,都必须要获得的授权
 *   ======================================================================
 *   修改者	：
 *   修改日期	：
 *   修改内容	：
 *   ======================================================================
 *
 ***************************************************************************/
#include "timapcacheworker.h"

#include <QSqlError>
#include <QSqlQuery>

#include "tilogger.h"
#include "timapengine.h"
#include "timapenginedata.h"

TiMapCacheWorker::TiMapCacheWorker() : m_db(nullptr), m_valid(false), m_isInit(false) { }

void TiMapCacheWorker::quit()
{
    //线程退出也清理,防止未启用线程前添加的任务不能被释放的问题
    QMutexLocker locker(&m_taskQueueMutex);
    clear();
    locker.unlock();
    if (this->isRunning())
        m_waitCond.wakeAll();
}

/**
 * @brief TiMapCacheWorker::enqueueTask 添加任务到队列
 * @param task
 * @return
 */
bool TiMapCacheWorker::enqueueTask(TiMapTask* task)
{
    //加锁,放入队列
    QMutexLocker locker(&m_taskQueueMutex);
    m_taskQueue.enqueue(task);

    //线程已运行,唤醒任务
    if (this->isRunning()) {
        m_waitCond.wakeAll();
    } else {
        locker.unlock();
        this->start(QThread::HighPriority);
    }
    return true;
}

void TiMapCacheWorker::setDBName(const QString& path)
{
    if (m_dbPath != path)
        m_dbPath = path;
}

void TiMapCacheWorker::run()
{
    //如果数据库为无效的,初始化数据库
    if (!m_isInit) {
        init();
    }

    if (m_isInit) {
        //打开数据库
        openDB();

        while (true) {
            QMutexLocker locker(&m_taskQueueMutex);
            if (!m_taskQueue.empty()) {
                auto task = m_taskQueue.dequeue();
                runTask(task);
                task->deleteLater();
            } else {
                //队列为空,等待5s,5s后解除阻塞,如果还为空则退出线程
                unsigned long timeoutMilliseconds = 5000;
                m_waitCond.wait(locker.mutex(), timeoutMilliseconds);
                if (m_taskQueue.empty())
                    break;
            }
        }

        //关闭数据库
        closeDB();
    } else {
        // init失败时,无法执行任务,需将队列中的任务删除
        QMutexLocker locker(&m_taskQueueMutex);
        clear();
    }
}

void TiMapCacheWorker::runTask(TiMapTask* task)
{
    switch (task->type()) {
    case TiMapTask::taskInit:
        break;
    case TiMapTask::taskTestInternet:
        break;
    case TiMapTask::taskCacheTile:
        saveTile(task);
        break;
    case TiMapTask::taskFetchTile:
        getTile(task);
        break;
    }
}

/**
 * @brief TiMapCacheWorker::clear 清空队列中的任务
 */
void TiMapCacheWorker::clear()
{
    while (!m_taskQueue.empty()) {
        auto task = m_taskQueue.dequeue();
        delete task;
    }
}

/**
 * @brief TiMapCacheWorker::init 初始化数据库,主要为了创建数据库表,创建完成后,会关闭
 * @return
 */
bool TiMapCacheWorker::init()
{
    m_isInit = false;
    if (!m_dbPath.isEmpty()) {

        if (openDB()) {
            m_isInit = createDB(*m_db);
        } else {
            LOG_WARN() << "can not open sql:" << m_db->lastError();
            m_valid = false;
        }
        //创建完后,关闭数据库
        closeDB();
    } else {
        LOG_CRIT() << "can not find sqlite path:" << m_dbPath;
        m_valid = false;
    }
    if (!m_isInit)
        m_valid = false;
    return m_valid;
}

static const QString kSession = QStringLiteral("QGeoTileWorkerSession");

/**
 * @brief TiMapCacheWorker::openDB 打开数据库
 * @return 打开失败返回false
 */
bool TiMapCacheWorker::openDB()
{
    if (!m_valid) {
        m_db.reset(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", kSession)));
        m_db->setDatabaseName(m_dbPath);
        m_db->setConnectOptions("QSQLITE_ENABLE_SHARED_CACHE");
        m_valid = m_db->open();
        LOG_DEBUG() << "open DB:" << m_valid << m_dbPath;
    }
    return m_valid;
}

/**
 * @brief TiMapCacheWorker::createDB 创建数据库 表头,此函数内可创建数据库内的所有表
 * @param db
 * @param createDefault
 * @return
 */
bool TiMapCacheWorker::createDB(QSqlDatabase& db, bool createDefault)
{
    bool      re = false;
    QSqlQuery query(db);
    re = query.exec("CREATE TABLE IF NOT EXISTS Tiles ("
                    "hash INTEGER PRIMARY KEY NOT NULL, "
                    "format TEXT NOT NULL, "
                    "tile BLOB NULL, "
                    "size INTEGER, "
                    "x INTEGER, "
                    "y INTEGER, "
                    "zoom INTEGER, "
                    "type INTEGER, "
                    "date INTEGER DEFAULT 0)");
    if (!re) {
        LOG_WARN() << "TiMap createDB Table error:" << query.lastError();
    }

    if (re) {
        re = query.exec("CREATE INDEX IF NOT EXISTS Tiles_index ON Tiles (hash)");
        if (!re) {
            LOG_WARN() << "TiMap createDB Index error:" << query.lastError();
        }
    }
    return re;
}

/**
 * @brief TiMapCacheWorker::closeDB 关闭数据库
 */
void TiMapCacheWorker::closeDB()
{
    if (m_db && m_db->isOpen()) {
        //关闭数据库
        m_db->close();
        m_db.reset();
        //移除数据库时需释放
        auto db = QSqlDatabase();
        if (db.contains(kSession)) {
            QSqlDatabase::removeDatabase(kSession);
        }
        m_valid = false;
    }
}

/**
 * @brief TiMapCacheWorker::testInternet 测试是否可以连接到internet
 * 弃用,现在TiMapEngine 中用定时器管理网络连接
 */
void TiMapCacheWorker::testInternet(TiMapTask* task)
{
    if (!m_hostLookupID) {
        m_hostLookupID = QHostInfo::lookupHost("www.baidu.com", this, [&](QHostInfo info) {
            m_hostLookupID = 0;
            auto _task     = static_cast<TiMapTestInternetTask*>(task);
            if (_task) {
                if (info.error() == QHostInfo::NoError) {
                    //                    emit sigInternetState(true);
                } else {
                    //                    emit sigInternetState(false);
                }
            }
        });
    }
}

/**
 * @brief TiMapCacheWorker::testTask 每个任务进来后，判断一下数据库是否可用
 * @param task
 * @return
 */
bool TiMapCacheWorker::testTask(TiMapTask* task)
{
    if (!m_valid) {
        task->setError("Database Error!");
        return false;
    }
    return true;
}

/**
 * @brief TiMapCacheWorker::getTile 从数据库中获取tile图
 * @param task
 */
void TiMapCacheWorker::getTile(TiMapTask* task)
{
    if (!testTask(task))
        return;
    auto _task = static_cast<TiMapFetchTileTask*>(task);
    bool found = false;
    if (_task) {
        QSqlQuery query(*m_db);
        QString   sql
                = QString("SELECT tile, format, size, x, y, zoom, type FROM Tiles WHERE hash = %1")
                          .arg(_task->hash());
        if (query.exec(sql)) {
            if (query.next()) {
                auto       img    = query.value(0).toByteArray();
                auto       format = query.value(1).toString();
                auto       size   = query.value(2).toUInt();
                auto       x      = query.value(3).toInt();
                auto       y      = query.value(4).toInt();
                auto       zoom   = query.value(5).toUInt();
                auto       type   = query.value(6).toString();
                TiMapTile* tile   = new TiMapTile(_task->hash());
                tile->setFormat(format);
                tile->setImg(img);
                tile->setSize(size);
                tile->setX(x);
                tile->setY(y);
                tile->setZoom(zoom);
                tile->setType(type);
                _task->setTileFetched(tile);
                found = true;
            }
        }

        if (!found) {
            //            LOG_WARN() << "not find tile hash:" << _task->hash();
            _task->setError("tile not in cache database!");
        }
    }
}

void TiMapCacheWorker::saveTile(TiMapTask* task)
{
    if (!testTask(task))
        return;
    auto _task = static_cast<TiMapSaveTileTask*>(task);
    if (_task) {
        QSqlQuery query(*m_db);
        query.prepare(
                "INSERT INTO Tiles(hash, format, tile, size, x, y, zoom, type, date) VALUES(?, "
                "?, ?, ?, ?, ?, ?, ?, ?)");
        query.addBindValue(_task->tile()->hash());
        query.addBindValue(_task->tile()->format());
        query.addBindValue(_task->tile()->img());
        query.addBindValue(_task->tile()->img().size());
        query.addBindValue(_task->tile()->x());
        query.addBindValue(_task->tile()->y());
        query.addBindValue(_task->tile()->zoom());
        query.addBindValue(_task->tile()->type());
        query.addBindValue(QDateTime::currentDateTime().toMSecsSinceEpoch());
        if (query.exec()) {
            //新的数据已存入
        } else {
            // Map数据已存在于数据库
        }
    }
}

