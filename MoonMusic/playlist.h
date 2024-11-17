#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QWidget>
#include "commonbutton.h"
#include "musicinfolistitem.h"
#include "playmodebutton.h"

namespace Ui {
class PlayList;
}

class PlayList : public QWidget
{
    Q_OBJECT
    CommonButton *btn_Clear;
    CommonButton *btn_Search;
    int currentRow;//当前播放音乐在播放队列中的行号
    PlayMode currentPlayMode;
public:
    QListWidget *listWidget;
    QList<MusicInfoListItem*> itemWidgetList;//存储待更新的UI(歌曲信息类)对象指针(播放队列)

public:
    explicit PlayList(QWidget *parent = nullptr);
    ~PlayList();

    void setLabel_Nums(int newNums);

    void deleteRow(int row);

    int getCurrentRow() const;

    void setCurrentRow(int newCurrentRow);

    PlayMode getCurrentPlayMode() const;
    void setCurrentPlayMode(PlayMode newCurrentPlayMode);

signals:

    void sendMedia(MusicInfoListItem* itemWidget);

public slots:

    void recPlayMode(PlayMode mode);

    void recMediaRequest(int index, int state);

private slots:

    void btnSearchClicked(bool checked);

    void btnClearClicked();

private:
    Ui::PlayList *ui;

    void getLastMedia(int index);

    void getIndexMedia(int index);

    void getNextMedia(int index);

    void getRandomMedia();
};

#endif // PLAYLIST_H
