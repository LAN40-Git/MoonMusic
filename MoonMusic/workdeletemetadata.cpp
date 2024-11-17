#include "workdeletemetadata.h"

#include <QSqlQuery>
#include <QSqlError>

WorkDeleteMetaData::WorkDeleteMetaData(QObject *parent)
    : QObject{parent}
{}

WorkDeleteMetaData::~WorkDeleteMetaData()
{

}

void WorkDeleteMetaData::stopWork()
{
    runStatus = false;
}

void WorkDeleteMetaData::run()
{
    QSqlQuery sql_query(db);
    QString sqlStatement;


    for (QString url : urlList) {
        sqlStatement = QString("delete from %1 where url = :url;").arg(playListName);
        sql_query.prepare(sqlStatement);
        sql_query.bindValue(":url",url);
        if (!runStatus) {
            emit workFinished();
            return;
        }
        if (!sql_query.exec()) {
            qDebug() << "歌曲删除失败：" << url;
            continue;
        }
    }
    emit workFinished();
}
