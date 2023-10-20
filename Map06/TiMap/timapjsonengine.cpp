/**************************************************************************
 *   文件名	：timapjsonengine.cpp
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
#include "timapjsonengine.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>

#include "tilogger.h"

TiMapJsonEngine::TiMapJsonEngine(const QString& path, QObject* parent) : QObject(parent)
{
    QDir dir("qrc:/");
    LOG_DEBUG() << dir.absolutePath();

    QFile loadFile(path);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        LOG_DEBUG() << "json read error" << path;
        return;
    }

    QByteArray saveData = loadFile.readAll();
    loadFile.close();

    QJsonParseError jsonError;
    QJsonDocument   loadDoc(QJsonDocument::fromJson(saveData, &jsonError));
    if (jsonError.error != QJsonParseError::NoError)
        return;

    m_jsonObj = loadDoc.object();
    LOG_DEBUG() << "json read success";
}

QString TiMapJsonEngine::getToken(const QString& provide)
{
    QString re;
    if (m_jsonObj.isEmpty())
        return re;
    if (m_jsonObj.contains(provide)) {
        auto sub = m_jsonObj.value(provide).toObject();
        re       = sub.value("token").toString();
    }
    return re;
}
