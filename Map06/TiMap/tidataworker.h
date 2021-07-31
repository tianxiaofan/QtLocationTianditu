/**************************************************************************
 *   文件名	：tidataworker.h
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

#pragma once
#include <QQueue>
#include <QSqlDatabase>
#include <QThread>
#include "TiMapTile.h"

class TiDataWorker : public QObject
{
    Q_OBJECT
public:
    TiDataWorker();
    ~TiDataWorker();

public slots:
    void writeSql(QSharedPointer<TiMapTile> tile);
    void readTile(QSharedPointer<TiMapTile> tile);
    void timeout();

private:
    bool openDB(const QString& name);

private:
    QSqlDatabase*                     m_db     = nullptr;
    bool                              m_valid  = false;
    QString                           m_dbName = "./MapCache.db";
    QQueue<QSharedPointer<TiMapTile>> m_queue;
    QTimer*                           m_timer;
    QSqlQuery*                        m_query = nullptr;
};
