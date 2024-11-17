#include "workreadmetadata.h"

WorkReadMetaData::WorkReadMetaData(QObject *parent)
    : QObject{parent}
{}

WorkReadMetaData::~WorkReadMetaData()
{

}

void WorkReadMetaData::stopWork()
{
    runStatus = false;
}


void WorkReadMetaData::run()
{
    QSqlQuery sql_query(db);
    QString sqlStatement;
    QByteArray byteArray;

    QString url;
    QString title;
    QString artist;
    QString album;
    QString copyright;
    QString date;
    QString lyrics;
    QString duration;
    QPixmap poster;
    int collStatus;

    sqlStatement = QString("select url,title,artist,album,copyright,date,lyrics,duration,poster,collStatus from %1;").arg(playListName);
    if (!sql_query.exec(sqlStatement)) {
        qDebug() << "无法更新歌单信息：" << playListName << sql_query.lastError();
        return;
    }
    while(sql_query.next()) {
        if (runStatus == false)
        {
            emit workFinished();

        }//利用标志位提前结束
        url = sql_query.value(0).toString();
        title = sql_query.value(1).toString();
        artist = sql_query.value(2).toString();
        album = sql_query.value(3).toString();
        copyright = sql_query.value(4).toString();
        date = sql_query.value(5).toString();
        lyrics = sql_query.value(6).toString();
        duration = sql_query.value(7).toString();
        byteArray = sql_query.value(8).toByteArray();
        collStatus = sql_query.value(9).toInt();

        poster.loadFromData(byteArray);

        if (poster.isNull()) {
            poster.load(":/icon/cd.png");
        }

        if (url.isNull()) {//过滤不存在的歌曲
            continue;
        }
        if (runStatus) {
            emit updateInfoRequest(url,playListName,poster,title,artist,album,duration,collStatus);
        } else {
            emit workFinished();
            return;
        }
    }
    emit workFinished();
}
