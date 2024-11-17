#ifndef METADATA_H
#define METADATA_H

#include <QString>
#include <QDebug>
#include <QMap>
#include "database.h"

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

class MetaData
{
public:
    MetaData();
    /**
     * @brief getMetaData 获取音频元数据
     * @param db 连接好的数据库对象（在控制层构造函数中创建）
     * @param url 音频url
     * @param playListName 歌单名
     * @param status 收藏状态
     * @return bool true：获取成功
     */
    static bool getMetaData(DataBase *db, const QString &url, const QString &playListName, bool status);
};

#endif // METADATA_H
