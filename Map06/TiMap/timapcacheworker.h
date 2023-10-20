/**************************************************************************
 *   文件名	：TiMapCacheWorker.h
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

#pragma once
#include <QHostInfo>
#include <QMutex>
#include <QQueue>
#include <QSqlDatabase>
#include <QThread>
#include <QWaitCondition>

class TiMapTask;
/**
 * @brief The TiMapCacheWorker class
 * /缓冲\读取 瓦片的线程
 */
class TiMapCacheWorker : public QThread
{
    Q_OBJECT
public:
    TiMapCacheWorker();

    void quit();
    bool enqueueTask(TiMapTask* task);
    void setDBName(const QString& path);

    // QThread interface
protected:
    void run() override;

private:
    void runTask(TiMapTask* task);
    void clear();
    bool init();
    bool openDB();
    bool createDB(QSqlDatabase& db, bool createDefault = true);
    void closeDB();
    void testInternet(TiMapTask* task);
    bool testTask(TiMapTask* task);
    void getTile(TiMapTask* task);
    void saveTile(TiMapTask* task);

private:
    QString                      m_dbPath;
    QMutex                       m_taskQueueMutex;
    QWaitCondition               m_waitCond;
    QQueue<TiMapTask*>           m_taskQueue;
    QScopedPointer<QSqlDatabase> m_db;
    std::atomic_bool m_valid; // 数据库是否有效,有效指数据库打开并已初始化化
    std::atomic_bool             m_isInit;
    int                          m_hostLookupID = 0;
};
