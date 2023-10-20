/**************************************************************************
 *   文件名	：timapurlengine.h
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-6-17
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

#pragma once
#include <QHash>
#include <QNetworkAccessManager>
#include <QObject>

class TiMapProvider;

/**
 * @brief The TiMapUrlEngine class 管理所有服务
 * 存放所有地图服务的 字符串及服务类的hash map
 */
class TiMapUrlEngine : public QObject
{
    Q_OBJECT
public:
    TiMapUrlEngine(QObject* parent = nullptr);

    QHash<QString, TiMapProvider*> getProviderTable() const { return m_provides; }
    QStringList                    getProviderNameList() const;
    QString                        getTypeFromId(const int id) const;
    int                            getIdFromType(const QString& type) const;
    TiMapProvider*                 getMapProviderFromId(int id) const;
    TiMapProvider*                 getMapProviderFromType(const QString& type) const;
    QNetworkRequest                getTileUrl(int id, int x, int y, int zoom,
                                              QNetworkAccessManager* networkManager) const;
    QNetworkRequest                getTileUrl(const QString& type, int x, int y, int zoom,
                                              QNetworkAccessManager* networkManager) const;
    QString                        getImageFormat(QString type, const QByteArray& image);
    QString                        getImageFormat(int id, const QByteArray& image);
    bool                           isElevation(int mapId);

private:
    QHash<QString, TiMapProvider*> m_provides;
};

