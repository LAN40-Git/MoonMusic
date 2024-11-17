#include "mymediaplayer.h"

#include <QDebug>

MyMediaPlayer::MyMediaPlayer(QObject *parent)
    : QMediaPlayer{parent}
{
    audioOutput = new QAudioOutput(this);
    setAudioOutput(audioOutput);
}

void MyMediaPlayer::setMedia(const QString &url)
{
    setSource(QUrl::fromLocalFile(url));//根据url设置音源
}

void MyMediaPlayer::setVolume(int value)
{
    double volume = value;
    audioOutput->setVolume(volume/100);
}
