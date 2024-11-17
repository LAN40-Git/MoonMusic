#ifndef MUSICINFOLISTITEM_H
#define MUSICINFOLISTITEM_H

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QPixmap>
#include <QSizePolicy>
#include <QEvent>
#include <QListWidget>
#include "collectbutton.h"
#include "mycheckbox.h"
#include "commonbutton.h"

class MusicInfoListItem : public QWidget
{
    Q_OBJECT
    int m_row;
    QString m_url;
    QString m_playListName;
    QPixmap m_poster;
    QString m_title;
    QString m_artist;
    QString m_album;
    QString m_duration;
    int m_collStatus;

    QHBoxLayout *h_layout;
    QVBoxLayout *v_layout;
    MyCheckBox *checkbox;//复选框
    QLabel *label_row;//行号
    QLabel *label_poster;//海报
    QLabel *label_title;//歌名
    QLabel *label_artist;//艺术家
    QLabel *label_album;//专辑
    QLabel *label_duration;//时长
    CollectButton *btn_collect;//收藏状态按钮
    CommonButton *btn_delete;

public:
    void init_CheckBox(const int &index);//初始化复选框

    void init_Label_Row(const int &index);//初始化行号

    void init_Label_Poster(const int &index);//初始化海报

    void init_Label_TitleAndLabel_Artist(const int &index);//初始化歌名和作者

    void init_Label_Album(const int &index);//初始化专辑

    void init_Label_Duration(const int &index);//初始化时长

    void init_Btn_Collect(const int &index);//初始化收藏按钮

    void init_Btn_Delete(const int &index);//初始化删除按钮

public:

    void init_MusicInfoItem();//初始化展示歌曲信息列表的项目

    void init_PlayListInfoItem();//初始化播放队列列表的项目

    /**
     * @brief MusicInfoListItem 初始化歌曲信息的构造函数
     * @param row 信息在列表中的序号
     * @param poster        海报
     * @param title         歌名
     * @param artist        艺术家
     * @param album         专辑
     * @param duration      时长
     * @param collStatus    收藏状态
     */
    MusicInfoListItem(const QString &url, const QString &playListName, int row, const QPixmap &poster,
                      const QString &title, QString &artist, const QString &album, const QString &duration, const int &collStatus);

    int row() const;
    void setRow(int newRow);

    QString playListName() const;
    void setPlayListName(const QString &newPlayListName);

    QPixmap poster() const;
    void setPoster(const QPixmap &newPoster);

    QString title() const;
    void setTitle(const QString &newTitle);

    QString artist() const;
    void setArtist(const QString &newArtist);

    QString album() const;
    void setAlbum(const QString &newAlbum);

    QString duration() const;
    void setDuration(const QString &newDuration);

    int collStatus() const;
    void setCollStatus(int newCollStatus);

    QString url() const;
    void setUrl(const QString &newUrl);

public slots:
    void onBtnDeleteClicked();

    void setNewBatchStatus(Qt::CheckState newstate);

    void recDeleteRow(bool status);

    void onCollStatusChg(const int &newCollStatus);

    void batchEnter();//进入批量操作

    void batchQuit(); //退出批量操作

signals:
    void sendDeleteRow_PlayList(int row);

    void sendDeleteRow(int row, const QString &url, bool status);

    void addToCollRequest(const QString &url);

    void delFromCollRequest(const int &row, const QString &url);
};

#endif // MUSICINFOLISTITEM_H
