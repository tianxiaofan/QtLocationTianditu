/**************************************************************************
 *   文件名	：tiqgeofiletilecache.cpp
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-5-13
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
#include "tiqgeofiletilecache.h"
#include "tilogger.h"
#include "timapengine.h"

#include <QSqlQuery>
#include <QSqlError>

TiQGeoFileTileCache::TiQGeoFileTileCache(const QString& directory, QObject* parent) :
    QGeoFileTileCache(directory, parent)
{
}

TiQGeoFileTileCache::~TiQGeoFileTileCache()
{
    if (m_db) {
        if (m_db->isOpen())
            m_db->close();
        delete m_db;
        m_db = nullptr;
    }
}

QSharedPointer<QGeoTileTexture> TiQGeoFileTileCache::get(const QGeoTileSpec& spec)
{
    QSharedPointer<QGeoTileTexture> tt = getFromMemory(spec);
    if (tt)
        return tt;
    //    return getFromSqlite(spec);
    return getFromDisk(spec);
}

void TiQGeoFileTileCache::insert(const QGeoTileSpec& spec, const QByteArray& bytes,
                                 const QString& format, CacheAreas areas)
{
    if (bytes.isEmpty())
        return;

    if (areas & QAbstractGeoTileCache::DiskCache) {
        //        QString filename = tileSpecToFilename(spec, format, directory_);
        //        addToSqlite(spec, format, bytes);
    }

    if (areas & QAbstractGeoTileCache::MemoryCache) {
        addToMemoryCache(spec, bytes, format);
    }

    /* inserts do not hit the texture cache -- this actually reduces overall
     * cache hit rates because many tiles come too late to be useful
     * and act as a poison */
}

void TiQGeoFileTileCache::addToSqlite(const QGeoTileSpec& spec, const QString& format,
                                      const QByteArray& bytes)
{
    if (!m_valid) {
        auto _connectId = TiMapEngine::instance()->getUrlEngine()->getTypeFromId(spec.mapId());
        m_valid         = openSqlite(_connectId);
    }
    if (!m_valid)
        return;

    QSqlQuery query(*m_db);
    query.prepare("INSERT INTO Tiles(hash, format, tile, size, x, y, zoom, type, date) VALUES(?, "
                  "?, ?, ?, ?, ?, ?, ?, ?)");
    // 参见QAbstractGeoTileCache类的头文件中 "qgeotilespec_p.h" 中包含 qHash
    //    unsigned int qHash(const QGeoTileSpec& spec)
    //    {
    //        unsigned int result = (qHash(spec.plugin()) * 13) % 31;
    //        result += ((spec.mapId() * 17) % 31) << 5;
    //        result += ((spec.zoom() * 19) % 31) << 10;
    //        result += ((spec.x() * 23) % 31) << 15;
    //        result += ((spec.y() * 29) % 31) << 20;
    //        result += (spec.version() % 3) << 25;
    //        return result;
    //    }
    query.addBindValue(getTileHash(spec.mapId(), spec.x(), spec.y(), spec.zoom()));
    query.addBindValue(format);
    query.addBindValue(bytes);
    query.addBindValue(bytes.size());
    query.addBindValue(spec.x());
    query.addBindValue(spec.y());
    query.addBindValue(spec.zoom());
    query.addBindValue(spec.mapId());
    query.addBindValue(QDateTime::currentDateTime().toMSecsSinceEpoch());
    if (query.exec()) {
        //新的数据已存入
    } else {
        // Map数据已存在于数据库
    }
}

QSharedPointer<QGeoTileTexture> TiQGeoFileTileCache::getFromSqlite(const QGeoTileSpec& spec)
{

    if (!m_valid) {
        auto _connectId = TiMapEngine::instance()->getUrlEngine()->getTypeFromId(spec.mapId());
        m_valid         = openSqlite(_connectId);
    }
    if (!m_valid)
        return QSharedPointer<QGeoTileTexture>();

    bool       found = false;
    QByteArray byte;
    QString    format;
    QSqlQuery  query(*m_db);
    QString    sql = QString("SELECT tile, format, type FROM Tiles WHERE hash = \"%1\"")
                          .arg(getTileHash(spec.mapId(), spec.x(), spec.y(), spec.zoom()));
    if (query.exec(sql)) {
        if (query.next()) {
            byte   = query.value(0).toByteArray();
            format = query.value(1).toString();
            found  = true;
        }
    }
    if (found) {
        QImage image;
        // Some tiles from the servers could be valid images but the tile fetcher
        // might be able to recognize them as tiles that should not be shown.
        // If that's the case, the tile fetcher should write "NoRetry" inside the file.
        if (isTileBogus(byte)) {
            QSharedPointer<QGeoTileTexture> tt(new QGeoTileTexture);
            tt->spec  = spec;
            tt->image = image;
            return tt;
        }

        // This is a truly invalid image. The fetcher should try again.
        if (!image.loadFromData(byte)) {
            handleError(spec, QLatin1String("Problem with tile image"));
            return QSharedPointer<QGeoTileTexture>(0);
        }

        // Converting it here, instead of in each QSGTexture::bind()
        if (image.format() != QImage::Format_RGB32 && image.format() != QImage::Format_ARGB32_Premultiplied)
            image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);

        addToMemoryCache(spec, byte, format);
        QSharedPointer<QGeoTileTexture> tt = addToTextureCache(spec, image);
        if (tt) {
            return tt;
        }
    }
    return QSharedPointer<QGeoTileTexture>();
}

quint64 TiQGeoFileTileCache::getTileHash(int type, int x, int y, int z)
{
    QString str   = QString().asprintf("%03d%08d%08d%03d", type, x, y, z);
    quint64 value = qHash(str);
    return value;
}

bool TiQGeoFileTileCache::openSqlite(const QString& connectId)
{
    if (m_db)
        return false;
    m_db           = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connectId));
    QString dbname = directory() + "/" + m_dbName;
    m_db->setDatabaseName(dbname);
    m_db->setConnectOptions("QSQLITE_ENABLE_SHARED_CACHE");
    bool re = m_db->open();
    if (!re) {
        LOG_DEBUG() << "sqlite can not open!";
        delete m_db;
        m_db = nullptr;
        return false;
    }
    LOG_INFO() << "open db:" << dbname;
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
        LOG_WARN() << "Map Cache Sql error(createDB Tiles):" << query.lastError().text();
    }
    return true;
}
