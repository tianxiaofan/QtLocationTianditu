/**************************************************************************
 *   文件名	：timapengine.h
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-6-16
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
#pragma once
#include <QObject>
#include <QTimer>

#include "timapcacheworker.h"
#include "timapenginedata.h"
#include "timapurlengine.h"

/**
 * @brief The TiMapEngine class 地图管理引擎,
 */
class TiMapEngine : public QObject
{

private:
    TiMapEngine(QObject* parent = nullptr);
    ~TiMapEngine();

public:
    static TiMapEngine* instance()
    {
        static TiMapEngine engine;
        return &engine;
    }

    /**
     * @brief getUrlEngine 返回url管理类,返回值永远不会为null
     * @return
     */
    TiMapUrlEngine* getUrlEngine() const { return m_mapUrlEngine; }

    TiMapFetchTileTask* createFetchTileTask(int mapId, int x, int y, int zoom);

    bool    internetActive() const { return m_internetState; }
    void    testInternet();
    QString getDBPath() const { return m_dbPath; }
    QString getDBName() const { return m_dbName; }

    void addTask(TiMapTask* task);
    void cacheTile(const QString& type, int x, int y, int zoom, const QByteArray& img,
                   const QString& format);
    void cacheTile(int mapId, int x, int y, int zoom, const QByteArray& img, const QString& format);

    bool isCache() const { return m_isCache; }
    void setCache(bool cache) { m_isCache = cache; }

    static quint32 getTileHash(int mapId, int x, int y, int zoom);
    std::atomic_int cacheCount;

private:
    void init();
    void addTile(const QString& type, int mapid, int x, int y, int zoom, const QByteArray& img,
                 const QString& format);

private slots:
    void onTimeout();

private:
    TiMapUrlEngine*  m_mapUrlEngine = nullptr;
    TiMapCacheWorker m_worker;
    std::atomic_bool m_internetState;
    QString          m_dbPath;
    QString          m_dbName;
    bool             m_isCache = true;
    QTimer           m_timer;
};
