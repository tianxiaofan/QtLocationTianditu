/**************************************************************************
 *   文件名	：tidataworker.cpp
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-7-31
 *   邮   箱	：499131808@qq.com
 *   Q Q		：499131808
 *   公   司
 *   功能描述      ：
 *   使用说明 ：任何人或机构要使用、修改、发布本源代码,
 *   ======================================================================
 *   修改者	：
 *   修改日期	：
 *   修改内容	：
 *   ======================================================================
 *
 ***************************************************************************/
#include "tidataworker.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include "timapengine.h"

TiDataWorker::TiDataWorker() { }

TiDataWorker::~TiDataWorker()
{
    if (m_db) {
        m_db->close();
        delete m_query;
    }
    if (m_timer)
        m_timer->deleteLater();
}

void TiDataWorker::writeSql(QSharedPointer<TiMapTile> tile)
{
    if (!m_valid) {
        m_valid = openDB(m_dbName);
    }
    if (!m_valid)
        return;
    if (!m_timer) {
        m_timer = new QTimer();
        connect(m_timer, &QTimer::timeout, this, &TiDataWorker::timeout);
        m_timer->start(10);
    }
    m_queue.append(tile);
}

void TiDataWorker::readTile(QSharedPointer<TiMapTile> tile)
{
    //    //读取时,如果发现query是活动的,则不进行读取
    //    if (!m_query || m_query->isActive())
    //        return;

    if (!m_valid) {
        m_valid = openDB(m_dbName);
    }
    if (m_valid) {
        QString sql
                = QString("SELECT tile, format, type FROM Tiles WHERE hash = \"%1\"").arg(tile->hash);
        if (m_query->exec(sql)) {
            if (m_query->next()) {
                tile->byte   = m_query->value(0).toByteArray();
                tile->format = m_query->value(1).toString();
                m_query->finish();
            }
        }
    }
}

void TiDataWorker::timeout()
{
    if (!m_query || m_query->isActive() || m_queue.isEmpty())
        return;

    qDebug() << m_queue.size() << "-----------";
    auto tile = m_queue.dequeue();
    m_query->prepare(
            "INSERT INTO Tiles(hash, format, tile, size, x, y, zoom, type, date) VALUES(?, "
            "?, ?, ?, ?, ?, ?, ?, ?)");
    m_query->addBindValue(tile->hash);
    m_query->addBindValue(tile->format);
    m_query->addBindValue(tile->byte);
    m_query->addBindValue(tile->byte.size());
    m_query->addBindValue(tile->x);
    m_query->addBindValue(tile->y);
    m_query->addBindValue(tile->zoom);
    m_query->addBindValue(tile->mapID);
    m_query->addBindValue(QDateTime::currentDateTime().toMSecsSinceEpoch());
    if (m_query->exec()) {
        //新的数据已存入
    } else {
        // Map数据已存在于数据库
    }
    m_query->finish();
}

bool TiDataWorker::openDB(const QString& name)
{
    if (m_db)
        return false;
    m_db           = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "tianditusql"));
    m_db->setDatabaseName(name);
    m_db->setConnectOptions("QSQLITE_ENABLE_SHARED_CACHE");
    bool re = m_db->open();
    if (!re) {
        qDebug() << "sqlite can not open!";
        delete m_db;
        m_db = nullptr;
        return false;
    }
    qDebug() << "open db:" << name;
    QSqlQuery query(*m_db);
    if (!query.exec("CREATE TABLE IF NOT EXISTS Tiles ("
                    "hash INTEGER PRIMARY KEY NOT NULL, "
                    "format TEXT NOT NULL, "
                    "tile BLOB NULL, "
                    "size INTEGER, "
                    "x INTEGER, "
                    "y INTEGER, "
                    "zoom INTEGER, "
                    "type INTEGER, "
                    "date INTEGER DEFAULT 0)")) {
        qDebug() << "Map Cache Sql error(createDB Tiles):" << query.lastError().text();
    }
    m_query = new QSqlQuery(*m_db);
    return true;
}
