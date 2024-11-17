#include "database.h"

#include <QDebug>
#include <QFileInfo>
#include <QImage>

DataBase::DataBase()
{
    createConnection("metaData"); //建立数据库连接接口
    db = QSqlDatabase::database("conToMetaData");//通过接口连接数据库
}

QString DataBase::getLyricsByUrl(const QString &url, const QString &playListName)
{
    QSqlQuery sql_query(db);
    QString sqlStatement;
    QString lyrics = "无歌词";

    sqlStatement = QString("select lyrics from %1 where url = :url;").arg(playListName);
    sql_query.prepare(sqlStatement);
    sql_query.bindValue(":url",url);
    if (!sql_query.exec()) {
        qDebug() << "无法获取歌词" << url;
        return lyrics;
    }
    if (sql_query.next()) {
        lyrics = sql_query.value(0).toString();
    }
    return lyrics;
}

bool DataBase::createPlayListNotExist(const QString &playListName)
{
    QSqlQuery sql_query(db);
    QString sqlStatement;

    //查询数据库中是否存在该歌单
    sql_query.prepare("select count(*) from sqlite_master where type='table' and name=:tableName;");
    sql_query.bindValue(":tableName",playListName);
    if (!sql_query.exec()) {
        qDebug() << "查找歌单失败：" << playListName << sql_query.lastError();
        return false;
    }

    if (sql_query.next()) {
        if (sql_query.value(0).toInt() == 0) {//如果不存在该歌单，则创建
            /*歌单表属性
             * 1.  id
             * 2.  url
             * 3.  title
             * 4.  artist
             * 5.  album
             * 6.  copyright
             * 7.  date
             * 8.  lyrics
             * 9.  duration
             * 10.  poster
             * 11. collStatus
             */
            sqlStatement = QString("create table %1 (id INTEGER PRIMARY KEY, url text not null, title text,"
                                   "artist text, album text, copyright text, date text, lyrics text,"
                                   "duration text, poster BLOB, collStatus int"
                                   ");").arg(playListName);
            if (!sql_query.exec(sqlStatement)) {
                qDebug() << "歌单创建失败：" << playListName << sql_query.lastError();
                return false;
            }
        }
    }

    return true;
}

bool DataBase::saveMetaData(const QMap<QString, QString> &metaDataMap, const QString &playListName, const QByteArray &byteArray, bool status)
{
    QSqlQuery sql_query(db);
    QString sqlStatement;

    QString url;
    QString title;
    QString artist;
    QString album;
    QString copyright;
    QString date;
    QString lyrics;
    QString duration;
    QByteArray poster;
    int collStatus=0;

    for (QMap<QString,QString>::const_iterator it = metaDataMap.constBegin(); it != metaDataMap.constEnd(); it++) {
        if      (it.key() == "url")         { url = it.value();}
        else if (it.key() == "title")       { title = it.value();}
        else if (it.key() == "artist")      { artist = it.value();}
        else if (it.key() == "album")       { album = it.value();}
        else if (it.key() == "copyright")   { copyright = it.value();}
        else if (it.key() == "date")        { date = it.value();}
        else if (it.key() == "LYRICS")      { lyrics = it.value();}
        else if (it.key() == "duration")    { duration = it.value();}
        else if (it.key() == "collStatus")  { collStatus = it.value().toInt();}
    }
    if (url.isNull()) {
        return false;
    }

    poster = byteArray;//获取以二进制形式存储的海报

    if (title.isNull()) {
        QFileInfo info(url);
        title = info.fileName();
        if (title.isNull()) {
            title = "未知歌名";
        }
    }
    if (artist.isNull()) {
        artist = "未知艺术家";
    }
    if (album.isNull()) {
        album = "未知专辑";
    }
    if (date.isNull()) {
        date = "未知发行期";
    }
    if (lyrics.isNull()) {
        lyrics = "无歌词";
    }

    //创建或检查歌单
    if (!createPlayListNotExist(playListName)) {
        qDebug() << "无法创建歌单：" << playListName;
        return false;
    }

    sqlStatement = QString("select count(*) from %1 where url = :url;").arg(playListName);
    sql_query.prepare(sqlStatement);
    sql_query.bindValue(":url",url);
    if (!sql_query.exec()) {
        qDebug() << "无法查找是否有相同的歌曲：" << url << sql_query.lastError();
        return false;
    } else if (sql_query.next()) {
        if (sql_query.value(0).toInt()>0) {
            status = false;
        }
    }

    if (status) {//如果保存工作要求发送更新UI信息的信号，则保持发送，否则只执行保存
        QPixmap pixmap;
        pixmap.loadFromData(poster);
        emit updateInfoRequest(url,playListName,pixmap,title,artist,album,duration,collStatus);
    }

    //删除歌单中具有相同路径的歌曲元数据
    sqlStatement = QString("delete from %1 where url = :url").arg(playListName);
    sql_query.prepare(sqlStatement);
    sql_query.bindValue(":url",url);
    if (!sql_query.exec()) {
        qDebug() << "无法清除相同源的歌曲元数据：" << url << sql_query.lastError();
        return false;
    }

    sqlStatement = QString("insert into %1 (url,title,artist,album,copyright,date,lyrics,duration,poster,collStatus)"
                           "values(:url,:title,:artist,:album,:copyright,:date,:lyrics,:duration,:poster,:collStatus);").arg(playListName);
    sql_query.prepare(sqlStatement);
    sql_query.bindValue(":url",url);
    sql_query.bindValue(":title",title);
    sql_query.bindValue(":artist",artist);
    sql_query.bindValue(":album",album);
    sql_query.bindValue(":copyright",copyright);
    sql_query.bindValue(":date",date);
    sql_query.bindValue(":lyrics",lyrics);
    sql_query.bindValue(":duration",duration);
    sql_query.bindValue(":poster",poster);
    sql_query.bindValue(":collStatus",collStatus);

    if (!sql_query.exec()) {
        qDebug() << "插入歌曲元数据失败：" << playListName << sql_query.lastError();
        return false;
    }

    // sqlStatement = QString("select count(*) from %1 where url = :url;").arg("全部音乐");
    // sql_query.prepare(sqlStatement);
    // sql_query.bindValue(":url",url);
    // if (!sql_query.exec()) {
    //     qDebug() << sql_query.lastError();
    // }
    // else if (sql_query.next()){
    //     if (sql_query.value(0).toInt()==0) {
    //         sqlStatement = QString("insert into %1 (url,title,artist,album,copyright,date,lyrics,duration,poster,collStatus)"
    //                                "values(:url,:title,:artist,:album,:copyright,:date,:lyrics,:duration,:poster,:collStatus);").arg("全部音乐");
    //         sql_query.prepare(sqlStatement);
    //         sql_query.bindValue(":url",url);
    //         sql_query.bindValue(":title",title);
    //         sql_query.bindValue(":artist",artist);
    //         sql_query.bindValue(":album",album);
    //         sql_query.bindValue(":copyright",copyright);
    //         sql_query.bindValue(":date",date);
    //         sql_query.bindValue(":lyrics",lyrics);
    //         sql_query.bindValue(":duration",duration);
    //         sql_query.bindValue(":poster",poster);
    //         sql_query.bindValue(":collStatus",collStatus);

    //         if (!sql_query.exec()) {
    //             qDebug() << "插入歌曲元数据失败：" << "全部音乐" << sql_query.lastError();
    //             return false;
    //         }
    //     }
    // }



    return true;
}

QStringList DataBase::getUrlFromPlayList(const QString &playListName)
{
    QSqlQuery sql_query(db);
    QString sqlStatement;

    QStringList urlList;

    sqlStatement = QString("select url from %1;").arg(playListName);
    if (!sql_query.exec(sqlStatement)) {
        qDebug() << "无法获取到歌单中所有歌曲的url：" << playListName << sql_query.lastError();
        return urlList;
    }

    while(sql_query.next()) {
        QString url = sql_query.value(0).toString();
        if (!url.isNull()) {
           urlList.append(url);
        }
    }
    return urlList;
}
