/**************************************************************************
 *   文件名	：tiqeoserviceproviderfactory.cpp
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-4-27
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
#include "tiqgeoserviceproviderfactory.h"
#include "tiqgeotiledmappingmanagerengine.h"


QT_BEGIN_NAMESPACE
TiQeoServiceProviderFactory::TiQeoServiceProviderFactory()
{
    
}

QGeoMappingManagerEngine* TiQeoServiceProviderFactory::createMappingManagerEngine(
        const QVariantMap& parameters, QGeoServiceProvider::Error* error, QString* errorString) const
{
    return new TiQGeoTiledMappingManagerEngine(parameters, error, errorString);
}

QT_END_NAMESPACE
