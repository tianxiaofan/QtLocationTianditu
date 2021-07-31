/**************************************************************************
 *   文件名	：tiqeoserviceproviderfactory.cpp
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
#include "tiqgeoserviceproviderfactory.h"
#include "tiqgeotiledmappingmanagerengine.h"
#include "tiqgeocodingmanagerengine.h"
#include <QDebug>

#ifdef MyMapPlugin_Static
Q_EXTERN_C Q_DECL_EXPORT const char* qt_plugin_query_metadata();
Q_EXTERN_C Q_DECL_EXPORT             QT_PREPEND_NAMESPACE(QObject) * qt_plugin_instance();

const QT_PREPEND_NAMESPACE(QStaticPlugin) qt_static_plugin_TiQeoServiceProviderFactory()
{
    QT_PREPEND_NAMESPACE(QStaticPlugin) plugin = { qt_plugin_instance, qt_plugin_query_metadata };
    return plugin;
}
#endif

QT_BEGIN_NAMESPACE
TiQeoServiceProviderFactory::TiQeoServiceProviderFactory()
{
    
}

QGeoMappingManagerEngine* TiQeoServiceProviderFactory::createMappingManagerEngine(
        const QVariantMap& parameters, QGeoServiceProvider::Error* error, QString* errorString) const
{
    return new TiQGeoTiledMappingManagerEngine(parameters, error, errorString);
}

QGeoCodingManagerEngine* TiQeoServiceProviderFactory::createGeocodingManagerEngine(
        const QVariantMap& parameters, QGeoServiceProvider::Error* error, QString* errorString) const
{
    return new TiQGeoCodingManagerEngine(parameters, error, errorString);
}
QT_END_NAMESPACE
