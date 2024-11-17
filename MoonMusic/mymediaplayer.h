#ifndef MYMEDIAPLAYER_H
#define MYMEDIAPLAYER_H

#include <QMediaPlayer>
#include <QAudioOutput>

class MyMediaPlayer : public QMediaPlayer
{
    Q_OBJECT
public:
    explicit MyMediaPlayer(QObject *parent = nullptr);

    QAudioOutput *audioOutput;

    void setMedia(const QString &url);

public slots:

    void setVolume(int value);

private slots:

};

#endif // MYMEDIAPLAYER_H
