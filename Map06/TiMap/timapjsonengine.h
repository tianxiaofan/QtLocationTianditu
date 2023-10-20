/**************************************************************************
 *   文件名	：timapjsonengine.h
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-6-29
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
#include <QJsonObject>
#include <QObject>

/**
 * @brief The TiMapJsonEngine class 暂时弃用
 */
class TiMapJsonEngine : public QObject
{
    Q_OBJECT
public:
    TiMapJsonEngine(const QString& path, QObject* parent = nullptr);
    QString getToken(const QString& provide);

private:
    QJsonObject m_jsonObj;
};

