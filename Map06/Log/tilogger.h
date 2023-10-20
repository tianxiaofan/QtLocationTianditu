/**************************************************************************
 *   文件名	：tilog.h
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

#pragma once
#include <QFile>
#include <QMutex>
#include <QObject>
#include <QTextStream>
#include <iostream>
#include <QDebug>
#include <QThread>

#define TILOGGER TiLogger::instance()
//重新定义qDebug相关宏
#undef QT_MESSAGELOG_FUNC
#define QT_MESSAGELOG_FUNC static_cast<const char*>(__FUNCTION__)
#undef QT_MESSAGELOG_FILE
#define QT_MESSAGELOG_FILE static_cast<const char*>(__FILE__)
#undef QT_MESSAGELOG_LINE
#define QT_MESSAGELOG_LINE __LINE__

#define THREAD QThread::currentThread()

//这里可参考qDebug的宏定义
#define LOG_DEBUG QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).debug
#define LOG_WARN QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).warning
#define LOG_INFO QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).info
#define LOG_CRIT QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).critical
#define LOG_FATAL QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).fatal

#define LOG_DEBUG_THREAD() LOG_DEBUG() << THREAD
#define LOG_WARN_THREAD() LOG_WARN() << THREAD
#define LOG_INFO_THREAD() LOG_INFO() << THREAD

class TiLogger : public QObject {
    Q_OBJECT
public:
    TiLogger();
    ~TiLogger();
    static TiLogger* instance();
signals:
    void sigLogger(const QString& log);

public:
    /// 是否启用将日志写入到文件,可选择日志存放日志
    void writeToFile(bool enable, const QString& path = "./Log/");
    /// 是否启用将日志通过信号的方式发送出
    void enableSignals(bool enable);
    // 是否启用日志输出到控制台
    void writeToConsole(bool enable);

    // 用于格式化日志，外部不要调用
    void writeMsg(QtMsgType type, const QMessageLogContext& context, const QString& msg);

private:
    bool        _isFile    = false;
    bool        _isSignals = false;
    bool        _isConsole = false;
    QMutex      m_mutex;
    QFile       m_file;
    QString     m_path = "./Log/";
    QTextStream m_textStream;
};
