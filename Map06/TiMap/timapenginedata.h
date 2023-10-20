/**************************************************************************
 *   文件名	：TiMapEngineData.h
 *   =======================================================================
 *   创 建 者	：田小帆
 *   创建日期	：2021-6-30
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
#include <QObject>
/**
 * @brief The TiMapCacheTile class 瓦片数据
 */
class TiMapTile : public QObject
{
    Q_OBJECT
public:
    explicit TiMapTile(uint hash) : m_hash(hash) { }

    uint       hash() const { return m_hash; }
    QByteArray img() const { return m_img; }
    int        size() const { return m_size; }
    QString    format() const { return m_format; }
    int        x() const { return m_x; }
    int        y() const { return m_y; }
    int        zoom() const { return m_zoom; }
    QString    type() const { return m_type; }
    int        mapId() const { return m_mapId; }

    void setImg(const QByteArray& img) { m_img = img; }
    void setSize(int size) { m_size = size; }
    void setFormat(const QString& format) { m_format = format; }
    void setX(int x) { m_x = x; }
    void setY(int y) { m_y = y; }
    void setZoom(int z) { m_zoom = z; }
    void setType(const QString& type) { m_type = type; }
    void setMapId(int id) { m_mapId = id; }

private:
    uint       m_hash = 0;  //由 x,y,zoom,type,计算得到的哈希值,具有tile的唯一性
    QByteArray m_img;       //瓦片具体数据
    int        m_size = 0;  //瓦片数据大小
    QString    m_format;    //瓦片格式
    int        m_x    = 0;  //瓦片x
    int        m_y    = 0;  //瓦片y
    int        m_zoom = 0;  //瓦片zoom
    QString    m_type;      //地图类型
    int        m_mapId = 0; //地图类型ID
};

/**
 * @brief The TiMapTask class 任务基类
 */
class TiMapTask : public QObject
{
    Q_OBJECT
public:
    enum TaskType { taskInit, taskTestInternet, taskCacheTile, taskFetchTile };
    Q_ENUM(TaskType)

    TiMapTask(TaskType type) : m_type(type) { }
    virtual ~TiMapTask() { }

    virtual TaskType type() { return m_type; }

    void setError(QString errorString = QString()) { emit error(type(), errorString); }

signals:
    void error(TiMapTask::TaskType type, QString errorString);

private:
    TaskType m_type;
};

/**
 * @brief The TiMapTestInternetTask class 测试网络连接
 * 弃用
 */
class TiMapTestInternetTask : public TiMapTask
{
    Q_OBJECT
public:
    TiMapTestInternetTask() : TiMapTask(TiMapTask::taskTestInternet) { }
};

/**
 * @brief The TiMapFetchTileTask class tile(瓦片)获取任务类
 */
class TiMapFetchTileTask : public TiMapTask
{
    Q_OBJECT
public:
    TiMapFetchTileTask(uint hash) : TiMapTask(TiMapTask::taskFetchTile), m_hash(hash) { }

    //设置已经获取到的tile
    void setTileFetched(TiMapTile* tile) { emit tileFetched(tile); }

    uint hash() const { return m_hash; }

signals:
    void tileFetched(TiMapTile* tile);

private:
    uint m_hash;
};

/**
 * @brief The TiMapSaveTileTask class tile 保存类
 */
class TiMapSaveTileTask : public TiMapTask
{
    Q_OBJECT
public:
    TiMapSaveTileTask(TiMapTile* tile) : TiMapTask(TiMapTask::taskCacheTile), m_tile(tile) { }
    ~TiMapSaveTileTask()
    {
        if (m_tile) {
            delete m_tile;
            m_tile = nullptr;
        }
    }

    TiMapTile* tile() const { return m_tile; }

private:
    TiMapTile* m_tile = nullptr;
};
