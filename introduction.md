# MoonMusic_音乐播放器项目



<img src="D:\MarkdownFiles\Qt_md\ProjectFiles\assets\MoonMusic_M8B7Dnd3xr.png" alt="MoonMusic_M8B7Dnd3xr" style="zoom:50%;" />

## 一. 项目概述

一个月不鹿，自学Qt做出的基于Qt6的音乐播放器。使用了Qt6、数据库、ffmpeg等技术。

## 二 .主要功能

### 我将我的音乐播放器分为下面几部分：

- 主页面

  - 导航栏

  - 歌单栏

  - 歌曲信息栏

  - 播放栏

  - 播放队列栏

  - 工具栏

- 歌词页面

  - 歌词显示栏

### 对应栏目的功能

- 主页面
  - 导航栏：
    - 点击Logo跳转到歌词页面
  - 歌单栏：
    - 点击歌单按钮可以切换歌单
    - 点击创建歌单按钮可以创建新的歌单
  - 歌曲信息栏：
    - 双击歌曲信息可播放音乐
    - 可收藏音乐
    - 可查看歌曲的基本信息
    - 惰性更新歌曲信息，不会造成页面卡顿
  - 播放栏：
    - 可以看到当前播放音乐的海报、歌名、艺术家、时长
    - 可以切换播放模式
      - 顺序播放
      - 单曲循环
      - 随机播放
    - 可以暂停音乐、播放下一首、上一首音乐
    - 可以设置播放器音量
    - 可以拖动进度条，改变音乐播放位置
    - 可以打开播放队列，查看对应的信息
  - 播放队列栏：
    - 可以清除播放队列
    - 可以收藏音乐
    - 可以逐个删除播放队列中的音乐
  - 工具栏
    - 可以添加当前歌单到播放队列
    - 可以导入音乐
    - 可以批量操作音乐
    - 可以搜索对应音乐

## 三. 主要难题及解决办法

### 1. 难题

1. 当同时导入大量音乐时，页面会直接无响应，直到导入完毕，甚至可能崩溃
2. 切换歌单时，但歌单音乐太多时，会造成卡顿

### 2. 解决办法：

#### 1) 采用多线程：

创建三个子类，都继承于QObject和QRunnable

一个类负责存储数据，一个类负责读取数据，一个类复制删除数据，在需要时创建一个线程运行这些任务，并加入线程池中，后台处理数据后发送信息到控制层，控制层将数据显示到页面。

三个任务应上互斥锁，防止资源竞争。

#### 2) 惰性更新页面

每次只更新用户看得见的数量，当用户滑动滚轮请求更多数据时，进行更新

### 3. 源文件

- 负责将获取导入音频数据并存入数据库的类

WorkSaveMetaData

.h

```c++
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

```

.cpp

```c++
#include "worksavemetadata.h"

void WorkSaveMetaData::stopWork()
{
    runStatus = false;
}

void WorkSaveMetaData::stopUpdate()
{
    updateStatus = false;//db中的saveMetaData函数停止发送更新UI信息信号
}

WorkSaveMetaData::WorkSaveMetaData(QObject *parent)
    : QObject{parent}
{}

WorkSaveMetaData::~WorkSaveMetaData()
{
    db = nullptr;
}

void WorkSaveMetaData::run()
{
    for (QString url : urlList) {
        if (!runStatus) { emit workFinished(playListName); return;}
        MetaData::getMetaData(db,url,playListName,updateStatus);
    }
    emit workFinished(playListName);
}
```

- 负责读取音频数据并发送到控制层的类

WorkReadMetaData

.h

```c++
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

```

.cpp

```c++
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
```

- 负责删除音频数据的类

WorkDeleteMetaData

.h

```c++
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

```

.cpp

```c++
#include "workdeletemetadata.h"

#include <QSqlQuery>
#include <QSqlError>

WorkDeleteMetaData::WorkDeleteMetaData(QObject *parent)
    : QObject{parent}
{}

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
```

## 四. 项目总结

### 1. 收获和感悟

  这个音乐播放器基本都是我自己完成的，包括图标的制作等等，我从这个音乐播放器学到了很多，包括Qt的信号与槽，重写事件以及事件过滤器，UI界面的布局管理，自定义继承于对应类的类，c++多线程编程，c++内存管理相关知识，c++的封装编程模式，ffmpeg获取音频元数据，Adobe Illustrator制作图标。

 <img src="D:\MarkdownFiles\Qt_md\ProjectFiles\assets\Illustrator_acMXWwU2XU.png" alt="Illustrator_acMXWwU2XU" style="zoom:33%;" />

### 2. 反思

  这个音乐播放器中仍然有许多漏洞与不足，比如线程之间导致的资源竞争，各种异常的处理，音频数据处理不够深入等，这些都可能导致程序崩溃，它们虽然让我很头疼，但是也让我对Qt编程开发的了解更加深入。

## 五. 界面展示

![MoonMusic_EG8jS70TYt](D:\MarkdownFiles\Qt_md\ProjectFiles\assets\MoonMusic_EG8jS70TYt.png)

![MoonMusic_TwNAldLsIa](D:\MarkdownFiles\Qt_md\ProjectFiles\assets\MoonMusic_TwNAldLsIa.png)
