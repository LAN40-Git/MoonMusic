#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QThread>
#include <QGraphicsBlurEffect>
#include <algorithm>
#include <vector>
#include <QEvent>
#include <QTimer>

#include "addplaylistpop.h"
#include "commonbutton.h"
#include "selectplaylistbutton.h"
#include "worksavemetadata.h"
#include "workreadmetadata.h"
#include "workdeletemetadata.h"
#include "warnpop.h"
#include "playlist.h"
#include "mymediaplayer.h"
#include "playmodebutton.h"
#include "volumebutton.h"

namespace Ui {
class PlayerControl;
}

enum class BatchMode {
    NumberMode=0,
    CheckBoxMode
};

class PlayerControl : public QWidget
{
    Q_OBJECT
    DataBase *db; //数据库操作对象指针
    int infoHasBeenUpdated = 0;
    QStringList collUrlList;//收藏歌单中歌曲的url，用来判断其它歌单中的歌曲是否被收藏，无需进入数据库更改
    std::vector<int> deleteRowList;//需要删除的歌曲信息的行号列表
    QStringList deleteUrlList;
    /* @CheckBox*/
    //批量操作全选框
    MyCheckBox *checkbox_SelectAll;
    /* @Button*/
    //选择歌单按钮
    SelectPlayListButton *btn_MyLove;   //"我的收藏"歌单按钮
    SelectPlayListButton *btn_LocalDL;  //"本地下载"歌单按钮
    SelectPlayListButton *btn_RecPlay;  //"最近播放"歌单按钮
    SelectPlayListButton *btn_AllMusic; //"全部音乐"歌单按钮
    CommonButton *btn_AddPlayList; //创建歌单按钮
    CommonButton *btn_ShowPlayList;//打开播放列表按钮
    CommonButton *btn_LastOne;
    QPushButton *btn_PlayOrPause;
    CommonButton *btn_NextOne;
    PlayModeButton *btn_PlayMode;
    VolumeButton *btn_Volume;
    VolumeWidget *widget_Volume;
    QPoint pos;
    QTimer *timer_Volume;
    int times = 0;
    QTimer *timer_Search;
    QTimer *timer_Search2;

    /* @ButtonGroup*/
    QButtonGroup *btnGroup_playList; //歌单栏按钮组

    QList<MusicInfoListItem*> itemWidgetList;//存储待更新的UI(歌曲信息类)对象指针
    QList<MusicInfoListItem*> tempList;
    PlayList *playlist;//播放列表
    MyMediaPlayer *myPlayer;

    QHBoxLayout *h_Layout_PlayerControl;//管理播放器控制栏按钮

    BatchMode batchMode;//批量操作模式

public:
    explicit PlayerControl(QWidget *parent = nullptr);

    ~PlayerControl();

    //初始化播放器
    void init_Player();

    //初始化播放列表
    void init_PlayList();

    //初始化创建歌单按钮
    void init_BasicButton();

    //初始化"我的音乐"按钮
    void init_MyMusicButton();

public slots:
    void onPositionChanged(qint64 pos);

    void onDurationChanged(qint64 duration);

    void onPlaybackStateChanged(QMediaPlayer::PlaybackState newState);

    void recMedia(MusicInfoListItem *itemWidget);

    void recDeleteRow_PlayList(int row);

    /**
     * @brief deletePlayListInfo
     */
    void deletePlayListInfo();

    /**
     * @brief receiveDeleteRow 实时接收复选框传来的行号
     * @param row 行号
     * @param status true：表示需要添加行号到删除列表
     */
    void recDeleteRow(int row, const QString &url, bool status);

    /**
     * @brief addToColl 添加歌曲到"我的收藏"
     * @param url 歌曲url
     * @param playListName "我的收藏"
     */
    void addToColl(const QString &url);

    /**
     * @brief delFromColl 将歌曲从"我的收藏"移除
     * @param row 歌曲所在行
     * @param url 歌曲url
     * @param playListName "我的收藏"
     */
    void delFromColl(const int &row, const QString &url);

    /**
     * @brief createPlayList 创建歌单
     * @param playListName 歌单名
     * @param path_Common  按钮普通图标
     * @param path_Hover   按钮鼠标悬浮时的图标
     */
    void createPlayList(const QString &playListName, const QString &path_Common, const QString &path_Hover);//创建歌单按钮

    /**
     * @brief on_btn_AddPlayList_clicked 创建歌单按钮点击事件
     * 创建一个弹窗以创建歌单
     * 给主窗口设置模态
     */
    void onBtnAddPlayListClicked();

    /**
     * @brief updatePlayListInfo 更新歌曲信息到UI界面
     * @param infoNums 需要更新的数量
     */
    void updatePlayListInfo(int infoNums);

    /**
     * @brief onPlayListChanged 歌单更改时，更换歌单名，并读取新的歌单信息到itemWidgetList
     * @param playListName 歌单名
     */
    void onPlayListChanged(const QString &playListName);//接收歌单更改的槽

    /**
     * @brief onUpdateInfoRequest 收集歌单信息，并加入待更新的歌曲信息列表
     * @param poster    海报
     * @param title     歌名
     * @param artist    艺术家
     * @param album     专辑
     * @param duration  时长
     * @param collStatus收藏状态
     */
    void onUpdateInfoRequest(QString url, QString playListName, QPixmap poster, QString title,
                             QString artist, QString album, QString duration, int collStatus);

    /**
     * @brief onWorkReadFinished 读取任务结束时执行
     */
    void onWorkReadFinished();

private slots:

    void onSliderTimeRelease();

    void onBtnLastOneClicked();

    void onBtnPlayOrPauseClicked();

    void onBtnNextOneClicked();

    void onTimerVolumeOut();

    void showPlayList(bool checked);

    /**
     * @brief updateVisibleItems 在检测到滚轮滚动的时候，更新UI
     */
    void updateVisibleItems();

    void on_btn_ImportTool_clicked();

    void on_btn_BatchTool_clicked();

    void on_btn_QuitBatch_clicked();

    void on_btn_DeleteTool_clicked();

    void on_listWidget_PlayListInfo_itemDoubleClicked(QListWidgetItem *item);

    void on_btn_PlayTool_clicked();

    void on_btn_Logo_clicked();

    void on_lineEdit_Search_textChanged(const QString &arg1);

    void on_btn_SearchTool_clicked();

signals:

    /**
     * @brief sendMediaRequest 向播放队列请求媒体
     * @param index 当前播放或需要播放的歌曲在播放队列中的下标
     * @param state -1表示请求上一首的媒体，0表示请求播放列表中下标为index的媒体，1表示请求下一首的媒体
     */
    void sendMediaRequest(int index, int state);

    void batchEnterRequest();//批量操作请求信号

    void batchQuitRequest(); //退出批量操作请求信号

    /**
     * @brief updatePlayListInfoRequest 请求主线程更新一定数量的歌曲信息
     * @param infoNums
     */
    void updatePlayListInfoRequest(const int &infoNums);

    void workSave_StopUpdate();

    void workSave_Stop();

    void workRead_Stop();

private:
    Ui::PlayerControl *ui;

    void saveMetaData(const QStringList &urlList, const QString &playListName, bool updateStatus);

    void delMetaData(std::vector<int> rowList, const QStringList &urlList, const QString &playListName);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // PLAYERCONTROL_H
