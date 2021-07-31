/**************************************************************************
 *   文件名	：tiqgeofiletilecache.cpp
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-5-13
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
#include "tiqgeofiletilecache.h"
#include <QDebug>
#include "timapengine.h"
#include "TiMapTile.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMetaObject>

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
    return getFromSqlite(spec);
    //    return getFromDisk(spec);
}

void TiQGeoFileTileCache::insert(const QGeoTileSpec& spec, const QByteArray& bytes,
                                 const QString& format, CacheAreas areas)
{
    if (bytes.isEmpty())
        return;

    if (areas & QAbstractGeoTileCache::DiskCache) {
        //        QString filename = tileSpecToFilename(spec, format, directory_);
        addToSqlite(spec, format, bytes);
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
    QSharedPointer<TiMapTile> tile = QSharedPointer<TiMapTile>(new TiMapTile);
    tile->hash   = TiMapEngine::getTileHash(spec.mapId(), spec.x(), spec.y(), spec.zoom());
    tile->format = format;
    tile->byte   = bytes;
    tile->x      = spec.x();
    tile->y      = spec.y();
    tile->zoom   = spec.zoom();
    tile->mapID  = spec.mapId();

    //不阻塞,这里主要是写入速度太慢
    QMetaObject::invokeMethod(TiMapEngine::instance()->getSqlThread(), "writeSql",
                              Qt::QueuedConnection, Q_ARG(QSharedPointer<TiMapTile>, tile));
}

QSharedPointer<QGeoTileTexture> TiQGeoFileTileCache::getFromSqlite(const QGeoTileSpec& spec)
{
    QSharedPointer<TiMapTile> tile = QSharedPointer<TiMapTile>(new TiMapTile);
    tile->hash = TiMapEngine::getTileHash(spec.mapId(), spec.x(), spec.y(), spec.zoom());

    auto start = clock();
    //阻塞等待返回,因为读取数据非常快,阻塞没有影响
    QMetaObject::invokeMethod(TiMapEngine::instance()->getSqlThread(), "readTile",
                              Qt::BlockingQueuedConnection, Q_ARG(QSharedPointer<TiMapTile>, tile));
    qDebug() << "read time:" << clock() - start;
    if (tile->byte.isEmpty())
        return QSharedPointer<QGeoTileTexture>();

    QImage image;
    // Some tiles from the servers could be valid images but the tile fetcher
    // might be able to recognize them as tiles that should not be shown.
    // If that's the case, the tile fetcher should write "NoRetry" inside the file.
    if (isTileBogus(tile->byte)) {
        QSharedPointer<QGeoTileTexture> tt(new QGeoTileTexture);
        tt->spec  = spec;
        tt->image = image;
        return tt;
    }

    // This is a truly invalid image. The fetcher should try again.
    if (!image.loadFromData(tile->byte)) {
        handleError(spec, QLatin1String("Problem with tile image"));
        return QSharedPointer<QGeoTileTexture>(0);
    }

    // Converting it here, instead of in each QSGTexture::bind()
    if (image.format() != QImage::Format_RGB32 && image.format() != QImage::Format_ARGB32_Premultiplied)
        image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    addToMemoryCache(spec, tile->byte, tile->format);
    QSharedPointer<QGeoTileTexture> tt = addToTextureCache(spec, image);
    if (tt) {
        return tt;
    }
    return QSharedPointer<QGeoTileTexture>();
}
