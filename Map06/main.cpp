﻿/**************************************************************************
 *   文件名	：%{Cpp:License:FileName}
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-7-31
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
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#ifdef MyMapPlugin_Static
#include <QtPlugin>
#include "tilogger.h"
#endif

int main(int argc, char *argv[])
{
    TILOGGER->writeToConsole(true);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

#ifdef MyMapPlugin_Static
    Q_IMPORT_PLUGIN(TiQeoServiceProviderFactory);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
            &app, [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
