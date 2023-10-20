/**************************************************************************
 *   文件名	：tilog.cpp
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-3-29
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
#include "tilogger.h"
#include <stdio.h>
#include <stdlib.h>

#include <QDateTime>
#include <QDir>
#include <QGlobalStatic>

Q_GLOBAL_STATIC(TiLogger, tiLogger) //注意这个是必须的

void outputMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    TiLogger::instance()->writeMsg(type, context, msg);
}

void TiLogger::writeMsg(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{

    QMutexLocker l(&m_mutex);
    //组合自定义数据
    static QString logType, sendmsg, writemsg;
    QDateTime      currentTime = QDateTime::currentDateTime();
    QString        logTime     = currentTime.toString("yyyy-MM-dd hh:mm:ss.zzz");
    const char*    fileName    = context.file ? context.file : "";
    const char*    function    = context.function ? context.function : "";
    QString        line        = QString::number(context.line);
    switch (type) {
    case QtDebugMsg:
        logType = "debug";
        break;
    case QtInfoMsg:
        logType = "info";
        break;
    case QtWarningMsg:
        logType = "warning";
        break;
    case QtCriticalMsg:
        logType = "critical";
        break;
    case QtFatalMsg:
        logType = "fatal";
        break;
    }
    writemsg = QString("[%1: %2 %3 %4 %5] %6").arg(logType, logTime, fileName, line, function, msg);

    if (_isFile) {
        QString newfileName
            = m_path
            + QString("%1_log.txt").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh"));
        if (m_file.fileName() != newfileName) {
            if (m_file.isOpen())
                m_file.close();
            m_file.setFileName(newfileName);
            m_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
            m_textStream.setDevice(&m_file);
            m_textStream.setCodec("UTF-8");
        }
        m_textStream << writemsg << Qt::endl;
        m_textStream.flush();
    }

    if (_isConsole) {
        fprintf(stdout, "%s\n", writemsg.toLocal8Bit().data());
        fflush(stdout);
    }

    if (_isSignals) {
        sendmsg = QString("[%1: %2] %3").arg(logType, logTime, msg);
        emit sigLogger(sendmsg);
    }
}

TiLogger::TiLogger()
{
    QMutexLocker l(&m_mutex);
    qInstallMessageHandler(outputMessage);
    QDir dir;
    if (!dir.exists(m_path))
        dir.mkpath(m_path);
}

TiLogger::~TiLogger()
{
    QMutexLocker l(&m_mutex);
    if (m_file.isOpen())
        m_file.close();
}

TiLogger* TiLogger::instance() { return tiLogger(); }

void TiLogger::writeToFile(bool enable, const QString &path)
{
    QMutexLocker l(&m_mutex);
    _isFile = enable;
    if (!path.isEmpty() && m_path != path)
        m_path = path;
}

void TiLogger::enableSignals(bool enable)
{
    QMutexLocker l(&m_mutex);
    _isSignals = enable;
}

void TiLogger::writeToConsole(bool enable)
{
    QMutexLocker l(&m_mutex);
    _isConsole = enable;
}
