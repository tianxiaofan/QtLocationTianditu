/**************************************************************************
 *   文件名	：timapengine.h
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-6-16
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
#pragma once
#include <QObject>

#include "timapurlengine.h"
#include "tidataworker.h"

/**
 * @brief The TiMapEngine class 地图管理引擎,
 */
class TiMapEngine : public QObject
{
    Q_OBJECT
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
    TiDataWorker*   getSqlThread() { return &m_data; }

    static quint32 getTileHash(int type, int x, int y, int z);

private:
    TiMapEngine(QObject* parent = nullptr);
    ~TiMapEngine();

private:
    TiMapUrlEngine* m_mapUrlEngine = nullptr;
    TiDataWorker    m_data;
    QThread         m_worker;
};
