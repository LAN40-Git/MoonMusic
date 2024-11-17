#ifndef DATABASE_H
#define DATABASE_H

#include "Connection.h"
#include "musicinfolistitem.h"
#include <QStringList>
#include <QMap>
#include <QImage>
#include <QByteArray>
#include <QList>
#include <QObject>

class DataBase : public QObject
{
    Q_OBJECT
public:
    QSqlDatabase db;
public:
    DataBase();

    /**
     * @brief getLyricsByUrl 根据歌曲url获取到歌词
     * @param url 歌曲url
     * @param playListName 歌单名
     * @return QString 歌词
     */
    QString getLyricsByUrl(const QString &url, const QString &playListName);

    /**
     * @brief createPlayListNotExist 创建数据库中不存在的歌单
     * @param playListName  歌单名
     * @return bool true：成功创建
     */
    bool createPlayListNotExist(const QString &playListName);

    /**
     * @brief saveMetaData 保存音频元数据到数据库
     * @param metaDataMap 存储元数据的键值对
     * @param playListName 歌单名
     * @param byteArray 海报的二进制形式
     * @param status 收藏状态
     * @return bool true：保存成功
     */
    bool saveMetaData(const QMap<QString, QString> &metaDataMap, const QString &playListName, const QByteArray &byteArray, bool status);

    /**
     * @brief getUrlFromPlayList 获取歌单中所有歌曲的url
     * @param playListName 歌单名
     * @return QStringList 歌单中所有歌曲的url
     */
    QStringList getUrlFromPlayList(const QString &playListName);
signals:
    /**
     * @brief updateInfoRequest 更新歌曲信息的信号
     * @param poster        海报
     * @param title         歌名
     * @param artist        艺术家
     * @param album         专辑
     * @param duration      时长
     * @param collStatus    收藏状态
     */
    void updateInfoRequest(QString url, QString playListName, QPixmap poster, QString title,
                           QString artist, QString album, QString duration, int collStatus);
};

#endif // DATABASE_H
