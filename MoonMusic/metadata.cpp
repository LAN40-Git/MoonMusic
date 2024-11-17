#include "metadata.h"

#include <QImage>
#include <QBuffer>
#include <QByteArray>
#include <QFileInfo>

MetaData::MetaData() {}

bool MetaData::getMetaData(DataBase *db, const QString &url, const QString &playListName, bool status)
{
    AVFormatContext *context = nullptr;
    AVDictionaryEntry *tag = nullptr;
    QMap<QString, QString> metaDataMap; //使用QMap存储相应信息

    int ret = avformat_open_input(&context, url.toUtf8().constData(), nullptr, nullptr);

    //判断是否打开音频文件
    if (ret != 0 || !context) {
        qDebug() << "无法打开音频文件：" << url;
        avformat_close_input(&context);
        return false;
    }

    //判断获取元数据是否成功
    if (avformat_find_stream_info(context,nullptr) != 0) {
        qDebug() << "获取音频元数据失败：" << url;
        avformat_close_input(&context);
        return false;
    }

    while((tag = av_dict_get(context->metadata,"",tag,AV_DICT_IGNORE_SUFFIX))) {
        if (tag) {
            QString keyString = tag->key;
            QString valueString = QString::fromUtf8(tag->value);
            if (valueString.isNull()) {
                if (keyString == "title") {
                    QFileInfo info(url);
                    valueString = info.fileName();
                }
                if (valueString.isNull()) {
                    valueString = "未知";
                }
            }
            metaDataMap.insert(keyString,valueString); //插入键值以及对应的数据
        }
    }


    //海报
    QImage image;
    int streamsCount = context->nb_streams;
    for (int i=0; i<streamsCount; ++i)
    {
        if (context->streams[i]->disposition & AV_DISPOSITION_ATTACHED_PIC)
        {
            AVPacket pkt = context->streams[i]->attached_pic;
            image = QImage::fromData((uchar*)pkt.data, pkt.size);
        }
    }
    if (image.isNull()) {//判断是否读取到图片
        image = QImage(":/icon/cd.png");//设置为默认图片
    }
    //QImage转二进制
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer,"JPG");

    //时长
    int64_t duration_ = 0;
    duration_ = context->duration;
    QString duration = QString::number(duration_);

    //收藏状态
    QString collStatus = "0";
    if (playListName == "我的收藏") { collStatus = "1";}

    metaDataMap.insert("duration", duration);
    metaDataMap.insert("url",url);
    metaDataMap.insert("collStatus",collStatus);

    db->saveMetaData(metaDataMap,playListName,byteArray,status);

    avformat_close_input(&context);
    return true;
}
