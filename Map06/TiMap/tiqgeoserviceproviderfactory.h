/**************************************************************************
 *   文件名	：tiqeoserviceproviderfactory.h
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-4-27
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
#include <QGeoServiceProviderFactory>

QT_BEGIN_NAMESPACE
class TiQeoServiceProviderFactory : public QObject, public QGeoServiceProviderFactoryV2
{
    Q_OBJECT
    Q_INTERFACES(QGeoServiceProviderFactoryV2)
    Q_PLUGIN_METADATA(IID "org.qt-project.qt.geoservice.serviceproviderfactory/5.0" FILE
                          "timap_plugin.json")
public:
    TiQeoServiceProviderFactory();

    // QGeoServiceProviderFactory interface
public:
    QGeoMappingManagerEngine* createMappingManagerEngine(const QVariantMap&          parameters,
                                                         QGeoServiceProvider::Error* error,
                                                         QString* errorString) const override;

    QGeoCodingManagerEngine* createGeocodingManagerEngine(const QVariantMap&          parameters,
                                                          QGeoServiceProvider::Error* error,
                                                          QString* errorString) const override;
};
QT_END_NAMESPACE
