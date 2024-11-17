#ifndef WORKREADMETADATA_H
#define WORKREADMETADATA_H

#include <QObject>
#include <QRunnable>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPixmap>
#include <QSqlError>

class WorkReadMetaData : public QObject, public QRunnable
{
    Q_OBJECT
    bool runStatus = true;//运行标志位
public:
    explicit WorkReadMetaData(QObject *parent = nullptr);
    ~WorkReadMetaData();
public:
    QString playListName;
    QSqlDatabase db;
public slots:
    void stopWork();
private:
    void run() override;
signals:
    void updateInfoRequest(QString url, QString playListName, QPixmap poster, QString title,
                           QString artist, QString album, QString duration, int collStatus);

    void workFinished();
};

#endif // WORKREADMETADATA_H
