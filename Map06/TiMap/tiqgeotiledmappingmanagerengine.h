/**************************************************************************
 *   文件名	：tiqgeotiledmappingmanagerengine.h
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
#include <QtLocation/QGeoServiceProvider>
#include <QtLocation/private/qgeotiledmappingmanagerengine_p.h>

class TiQGeoTiledMappingManagerEngine : public QGeoTiledMappingManagerEngine
{
public:
    TiQGeoTiledMappingManagerEngine(const QVariantMap&          parameters,
                                    QGeoServiceProvider::Error* error, QString* errorString);
    ~TiQGeoTiledMappingManagerEngine();

    QGeoMap* createMap() override;

private:
    void setCache(const QVariant& parameters);
};
