#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

static bool createConnection(QString dataBase_Name)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "conToMetaData");
    db.setDatabaseName(QString("%1%2").arg(dataBase_Name,".db"));

    //判断是否连接成功
    if (!db.open()) { //未连接到数据库
        qDebug() << "Error: Failed to connect database." << db.lastError();
        return false;
    }
    else { //连接到数据库
        return true;
    }
}


#endif // CONNECTION_H
