#ifndef WORKSAVEMETADATA_H
#define WORKSAVEMETADATA_H

#include <QObject>
#include <QRunnable>
#include "metadata.h"

class WorkSaveMetaData : public QObject, public QRunnable
{
    Q_OBJECT
    bool runStatus = true;//运行标志位
    bool updateStatus = true;//控制DataBase对象的updateInfoRequest信号
public:
    QStringList urlList;
    QString playListName;
    DataBase *db;

public slots:
    void stopWork();
    void stopUpdate();
public:
    explicit WorkSaveMetaData(QObject *parent = nullptr);
    ~WorkSaveMetaData();

private:
    void run() override;

signals:
    void workFinished(const QString &playListName);
};

#endif // WORKSAVEMETADATA_H
