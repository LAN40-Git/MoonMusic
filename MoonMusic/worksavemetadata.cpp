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
