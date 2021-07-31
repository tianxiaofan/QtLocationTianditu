/**************************************************************************
 *   文件名	：timapurlengine.cpp
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-6-17
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
#include "timapurlengine.h"
#include "titianditumapprovider.h"
#include <QDebug>

TiMapUrlEngine::TiMapUrlEngine(QObject* parent) : QObject(parent)
{
    m_provides["Tianditu Satellite"] = new TiTiandituImgMapProvider(this); //天地图卫星图
    m_provides["Tianditu Street"] = new TiTiandituCiaMapProvider(this); //天地图路网标注信息图
}

QStringList TiMapUrlEngine::getProviderNameList() const
{
    return m_provides.keys();
}

QString TiMapUrlEngine::getTypeFromId(const int id) const
{
    auto i = m_provides.constBegin();
    while (i != m_provides.constEnd()) {
        if ((int)(qHash(i.key()) >> 1) == id)
            return i.key();
        ++i;
    }
    return QString();
}

// qhash为uint,转int会超int范围,右移一位缩小一倍
int TiMapUrlEngine::getIdFromType(const QString& type) const
{
    return (int)(qHash(type) >> 1);
}

TiMapProvider* TiMapUrlEngine::getMapProviderFromId(int id) const
{
    QString _type = getTypeFromId(id);
    return getMapProviderFromType(_type);
}

TiMapProvider* TiMapUrlEngine::getMapProviderFromType(const QString& type) const
{
    if (!type.isEmpty()) {
        if (m_provides.find(type) != m_provides.end())
            return m_provides[type];
    }
    return nullptr;
}

QNetworkRequest TiMapUrlEngine::getTileUrl(int id, int x, int y, int zoom,
                                           QNetworkAccessManager* networkManager) const
{
    //    qDebug() << getTypeFromId(id) << id;
    auto _provide = getMapProviderFromId(id);
    if (_provide)
        return _provide->getTileURL(x, y, zoom, networkManager);

    qDebug() << "map not registered:" << getTypeFromId(id);
    return QNetworkRequest(QUrl());
}

QNetworkRequest TiMapUrlEngine::getTileUrl(const QString& type, int x, int y, int zoom,
                                           QNetworkAccessManager* networkManager) const
{
    auto _provide = getMapProviderFromType(type);
    if (_provide)
        return _provide->getTileURL(x, y, zoom, networkManager);
    qDebug() << "map not registered:" << type;
    return QNetworkRequest(QUrl());
}
