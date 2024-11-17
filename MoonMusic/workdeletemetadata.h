#ifndef WORKDELETEMETADATA_H
#define WORKDELETEMETADATA_H

#include <QObject>
#include <QRunnable>
#include <QSqlDatabase>

class WorkDeleteMetaData : public QObject,public QRunnable
{
    Q_OBJECT
    bool runStatus = true;//运行标志位
public:
    explicit WorkDeleteMetaData(QObject *parent = nullptr);
    ~WorkDeleteMetaData();
public:
    QStringList urlList;
    QString playListName;
    QSqlDatabase db;

public slots:
    void stopWork();

private:
    void run() override;

signals:
    void workFinished();
};

#endif // WORKDELETEMETADATA_H
