/**************************************************************************
 *   文件名	：timapengine.cpp
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
#include "timapengine.h"
#include <QDebug>

quint32 TiMapEngine::getTileHash(int type, int x, int y, int z)
{
    QString str = QString().asprintf("%03d%08d%08d%03d", type, x, y, z);
    return qHash(str);
}

TiMapEngine::TiMapEngine(QObject* parent) : QObject(parent)
{
    m_mapUrlEngine = new TiMapUrlEngine(this);
    qDebug() << "instance !";
    m_data.moveToThread(&m_worker);
    m_worker.start();
}

TiMapEngine::~TiMapEngine()
{
    m_worker.quit();
    m_worker.wait();
}
