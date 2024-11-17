#include "playercontrol.h"
#include "ui_playercontrol.h"

#include "metadata.h"

#include <QListWidgetItem>
#include <QFileDialog>
#include <QThreadPool>
#include <QScrollBar>
#include <QStringList>

PlayerControl::PlayerControl(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlayerControl)
{
    ui->setupUi(this);
    db = new DataBase();//初始化数据库操作指针对象
    //setWindowFlags(Qt::FramelessWindowHint);
    setWindowTitle("MoonMusic");
    setWindowIcon(QIcon(":/icon/logo.png"));
    batchMode = BatchMode::NumberMode;
    ui->widget_Tool->hide();

    //初始化歌单栏按钮组
    btnGroup_playList = new QButtonGroup(this);
    btnGroup_playList->setExclusive(true);


    init_MyMusicButton();   //初始化"我的音乐"按钮
    init_BasicButton();     //初始化创建歌单按钮
    init_PlayList();        //初始化播放列表
    init_Player();

    timer_Volume = new QTimer(this);
    connect(timer_Volume,&QTimer::timeout,this,&PlayerControl::onTimerVolumeOut);
    timer_Search = new QTimer(this);
    timer_Search2 = new QTimer(this);
    connect(timer_Search,&QTimer::timeout,[=](){
        if (ui->lineEdit_Search->width() <= 0) {
            timer_Search->stop();
        }
        else {
            ui->lineEdit_Search->setFixedWidth(ui->lineEdit_Search->width()-1);
        }
    });

    connect(timer_Search2,&QTimer::timeout,[=](){
        if (ui->lineEdit_Search->width() >= 220) {
            timer_Search2->stop();
        }
        else {
            ui->lineEdit_Search->setFixedWidth(ui->lineEdit_Search->width()+1);
        }
    });

    collUrlList = db->getUrlFromPlayList("我的收藏");//获取"我的收藏"歌单的所有歌曲url
    ui->listWidget_PlayListInfo->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    connect(db,&DataBase::updateInfoRequest,this,&PlayerControl::onUpdateInfoRequest);//接收更新歌单列表的信号
    connect(ui->listWidget_PlayListInfo->verticalScrollBar(),&QScrollBar::valueChanged,this,&PlayerControl::updateVisibleItems);//滚动更新UI
    connect(this,&PlayerControl::updatePlayListInfoRequest,this,&PlayerControl::updatePlayListInfo);//更新一定数量的歌曲信息
    connect(ui->horizontalSliderTime,&QSlider::sliderReleased,this,&PlayerControl::onSliderTimeRelease);
}


PlayerControl::~PlayerControl()
{
    db->db.close();
    delete db;
    delete ui;
}

void PlayerControl::init_Player()
{
    myPlayer = new MyMediaPlayer(this);
    connect(widget_Volume,&VolumeWidget::sendSliderValue,myPlayer,&MyMediaPlayer::setVolume);
    connect(myPlayer,&MyMediaPlayer::playbackStateChanged,this,&PlayerControl::onPlaybackStateChanged);
    connect(myPlayer,&MyMediaPlayer::positionChanged,this,PlayerControl::onPositionChanged);
    connect(myPlayer,&MyMediaPlayer::durationChanged,this,&PlayerControl::onDurationChanged);
    myPlayer->setVolume(50);
}

void PlayerControl::init_PlayList()
{
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);//实例阴影shadow
    shadow->setOffset(0, 0);//设置阴影距离
    shadow->setColor(QColor("#444444"));//设置阴影颜色
    shadow->setBlurRadius(20);//设置阴影圆角
    playlist = new PlayList(this);
    playlist->move(width()-playlist->width(),60);
    playlist->hide();
    playlist->setGraphicsEffect(shadow);
    playlist->setContentsMargins(0,0,1,1);
    connect(this,&PlayerControl::sendMediaRequest,playlist,&PlayList::recMediaRequest);
    connect(btn_PlayMode,&PlayModeButton::sendPlayMode,playlist,&PlayList::recPlayMode);
    connect(playlist,&PlayList::sendMedia,this,&PlayerControl::recMedia);
    btn_PlayMode->init_PlayMode(PlayMode::SequenceMode);
}


void PlayerControl::init_BasicButton()
{
    //初始化创建歌单按钮
    btn_AddPlayList = new CommonButton(":/icon/addplaylist_grey.png",
                                       ":/icon/addplaylist_hover.png",
                                       ":/icon/addplaylist_pressed.png",
                                       QSize(13,12),QSize(30,30));
    btn_AddPlayList->setParent(ui->widget_CreateMusic);
    btn_AddPlayList->move(125,0);
    connect(btn_AddPlayList,&CommonButton::clicked,this,&PlayerControl::onBtnAddPlayListClicked);//按钮单击事件信号槽
    //初始化批量操作中的全选框
    checkbox_SelectAll = new MyCheckBox();
    checkbox_SelectAll->setParent(ui->widget_BatchOperation);
    checkbox_SelectAll->move(11,60);
    //初始化打开播放列表按钮
    btn_ShowPlayList = new CommonButton(":/icon/showlist.png",":/icon/showlist2.png",":/icon/showlist3.png",QSize(23,23),QSize(40,40));
    btn_ShowPlayList->setParent(ui->widget_ShowPlayList);
    btn_ShowPlayList->move(10,10);
    btn_ShowPlayList->setCheckable(true);
    connect(btn_ShowPlayList,&CommonButton::clicked,this,&PlayerControl::showPlayList);
    //初始化播放控制栏按钮
    btn_PlayMode = new PlayModeButton;
    btn_LastOne = new CommonButton(":/icon/lastone.png",":/icon/lastone.png",":/icon/lastone.png",QSize(15,15),QSize(30,30));
    connect(btn_LastOne,&CommonButton::clicked,this,&PlayerControl::onBtnLastOneClicked);
    btn_PlayOrPause = new QPushButton;
    btn_PlayOrPause->setFixedSize(30,30);
    btn_PlayOrPause->setIconSize(QSize(27,27));
    btn_PlayOrPause->setIcon(QIcon(":/icon/play.png"));
    btn_PlayOrPause->setStyleSheet("border:none;");
    btn_PlayOrPause->setCursor(Qt::PointingHandCursor);
    connect(btn_PlayOrPause,&QPushButton::clicked,this,&PlayerControl::onBtnPlayOrPauseClicked);
    btn_NextOne = new CommonButton(":/icon/nextone.png",":/icon/nextone.png",":/icon/nextone.png",QSize(15,15),QSize(30,30));
    connect(btn_NextOne,&CommonButton::clicked,this,&PlayerControl::onBtnNextOneClicked);
    btn_Volume = new VolumeButton(this);
    btn_Volume->installEventFilter(this);
    btn_Volume->recSliderValue(50);
    widget_Volume = new VolumeWidget;
    widget_Volume->installEventFilter(this);
    widget_Volume->setParent(this);
    widget_Volume->hide();
    widget_Volume->init_Slider(50);
    widget_Volume->init_Label();
    connect(widget_Volume,&VolumeWidget::sendSliderValue,btn_Volume,&VolumeButton::recSliderValue);
    h_Layout_PlayerControl = new QHBoxLayout;
    h_Layout_PlayerControl->addWidget(btn_PlayMode);
    h_Layout_PlayerControl->addWidget(btn_LastOne);
    h_Layout_PlayerControl->addWidget(btn_PlayOrPause);
    h_Layout_PlayerControl->addWidget(btn_NextOne);
    h_Layout_PlayerControl->addWidget(btn_Volume);
    ui->widget_Bottom_Part2->setLayout(h_Layout_PlayerControl);
}


void PlayerControl::init_MyMusicButton()
{
    QVBoxLayout *v_MyMusicLayout = new QVBoxLayout(ui->widget_MyMusic); //垂直布局
    // 初始化按钮
    btn_MyLove = new SelectPlayListButton("我的收藏",":/icon/heart.png", ":/icon/heart2.png");
    btn_LocalDL = new SelectPlayListButton("本地下载",":/icon/download.png", ":/icon/download2.png");
    btn_RecPlay = new SelectPlayListButton("最近播放",":/icon/recplay.png", ":/icon/recplay2.png");
    btn_AllMusic = new SelectPlayListButton("全部音乐",":/icon/allmusic.png", ":/icon/allmusic2.png");
    // 将按钮加入歌单栏按钮组
    btnGroup_playList->addButton(btn_MyLove);
    btnGroup_playList->addButton(btn_LocalDL);
    btnGroup_playList->addButton(btn_RecPlay);
    btnGroup_playList->addButton(btn_AllMusic);
    // 创建四个基本歌单(如果不存在)
    db->createPlayListNotExist("我的收藏");
    db->createPlayListNotExist("本地下载");
    db->createPlayListNotExist("最近播放");
    db->createPlayListNotExist("全部音乐");
    // 为按钮连接信号与槽
    connect(btn_MyLove,&SelectPlayListButton::playListChanged,this,&PlayerControl::onPlayListChanged);
    connect(btn_LocalDL,&SelectPlayListButton::playListChanged,this,&PlayerControl::onPlayListChanged);
    connect(btn_RecPlay,&SelectPlayListButton::playListChanged,this,&PlayerControl::onPlayListChanged);
    connect(btn_AllMusic,&SelectPlayListButton::playListChanged,this,&PlayerControl::onPlayListChanged);
    // 将控件添加到垂直布局中
    v_MyMusicLayout->addWidget(ui->label_MyMusic);
    v_MyMusicLayout->addWidget(btn_MyLove);
    v_MyMusicLayout->addWidget(btn_LocalDL);
    v_MyMusicLayout->addWidget(btn_RecPlay);
    v_MyMusicLayout->addWidget(btn_AllMusic);
}

void PlayerControl::onPositionChanged(qint64 pos)
{
    if (!ui->horizontalSliderTime->isSliderDown()) {
        ui->horizontalSliderTime->setSliderPosition(pos);
        int sec = pos/1000;
        int min = sec/60;
        sec = sec%60;
        QString drtTime = QString("%1%2:%3%4").arg(QString::number(min/10),QString::number(min%10),QString::number(sec/10),QString::number(sec%10));
        ui->label_Pos->setText(drtTime);
        if (pos == ui->horizontalSliderTime->maximum()) {
            if (playlist->getCurrentPlayMode() == PlayMode::SingleLoopMode) {
                myPlayer->stop();
                myPlayer->play();
            }
            else {
                emit sendMediaRequest(playlist->getCurrentRow(),1);
            }

        }
    }
}

void PlayerControl::onDurationChanged(qint64 duration)
{
    ui->horizontalSliderTime->setMaximum(duration);
}

void PlayerControl::onPlaybackStateChanged(QMediaPlayer::PlaybackState newState)
{
    if (newState == QMediaPlayer::PlayingState) {
        btn_PlayOrPause->setIcon(QIcon(":/icon/stop.png"));
    }
    else if (newState == QMediaPlayer::PausedState) {
        btn_PlayOrPause->setIcon(QIcon(":/icon/play.png"));
    }
    else if (newState == QMediaPlayer::StoppedState) {
        btn_PlayOrPause->setIcon(QIcon(":/icon/stop.png"));
        if (playlist->getCurrentPlayMode() == PlayMode::SingleLoopMode) {
            myPlayer->play();
        }
    }
}

void PlayerControl::recMedia(MusicInfoListItem* itemWidget)
{
    QString url = itemWidget->url();
    QString playListName = itemWidget->playListName();
    QString lyrics = db->getLyricsByUrl(url,playListName);
    QStringList lyricsList = lyrics.split('\n');
    ui->listWidget_Lyrics->clear();
    for (QString lyric : lyricsList) {
        QListWidgetItem *item = new QListWidgetItem(lyric);
        ui->listWidget_Lyrics->addItem(item);
    }
    if (myPlayer->mediaStatus() == QMediaPlayer::NoMedia) {
        myPlayer->setMedia(url);
        myPlayer->play();
        QString title = itemWidget->title();
        QString artist = itemWidget->artist();
        QPixmap pixmap = itemWidget->poster();
        QString duration = itemWidget->duration();
        int64_t drtTime; //计算分秒显示的时长
        drtTime = duration.toInt();
        int sec = drtTime/1000000;
        int min = sec/60;
        sec %= 60;
        QString p_duration = QString("%1%2:%3%4").arg(QString::number(min/10),QString::number(min%10),
                                                      QString::number(sec/10),QString::number(sec%10));
        ui->label_Title->setText(title);
        ui->label_Artist->setText(artist);
        ui->label_Poster->setPixmap(pixmap);
        ui->label_Duration->setText("/"+p_duration);
        ui->label_Poster->setScaledContents(true);
    }
    else {
        if (QUrl::fromLocalFile(url) == myPlayer->source()) {//如果请求到的媒体与当前媒体相同，则从头播放
            myPlayer->play();
        }
        else {
            QString url = itemWidget->url();
            myPlayer->setMedia(url);
            myPlayer->play();
            QString title = itemWidget->title();
            QString artist = itemWidget->artist();
            QPixmap pixmap = itemWidget->poster();
            QString duration = itemWidget->duration();
            int64_t drtTime; //计算分秒显示的时长
            drtTime = duration.toInt();
            int sec = drtTime/1000000;
            int min = sec/60;
            sec %= 60;
            QString p_duration = QString("%1%2:%3%4").arg(QString::number(min/10),QString::number(min%10),
                                                          QString::number(sec/10),QString::number(sec%10));
            ui->label_Title->setText(title);
            ui->label_Artist->setText(artist);
            ui->label_Poster->setPixmap(pixmap);
            ui->label_Duration->setText("/"+p_duration);
            ui->label_Poster->setScaledContents(true);
        }
    }
}


void PlayerControl::recDeleteRow_PlayList(int row)
{
    playlist->deleteRow(row);
}


void PlayerControl::deletePlayListInfo()
{
    delMetaData(deleteRowList,deleteUrlList,ui->label_PlayListName->text());
    deleteUrlList.clear();
    deleteRowList.clear();
    QString text = QString("已选%1首").arg(QString::number(deleteRowList.size()));
    ui->label_BatchNums->setText(text);
    checkbox_SelectAll->setCheckState(Qt::Unchecked);
    checkbox_SelectAll->setIcon_Now(":/icon/rect_grey.png");
}

void PlayerControl::recDeleteRow(int row, const QString &url, bool status)
{
    //qDebug() << row << url;
    if (status) {
        deleteRowList.push_back(row);
        deleteUrlList.append(url);
    }
    else {
        auto it = std::find(deleteRowList.begin(), deleteRowList.end(), row);
        if (it != deleteRowList.end()) {
            deleteRowList.erase(it);
        }
        deleteUrlList.removeAll(url);
    }
    QString text = QString("已选%1首").arg(QString::number(deleteRowList.size()));
    ui->label_BatchNums->setText(text);
    //qDebug() << deleteRowList.size();
}


void PlayerControl::addToColl(const QString &url)
{
    QStringList urlList;
    urlList.append(url);
    collUrlList.append(url);
    saveMetaData(urlList,"我的收藏",0);
}


void PlayerControl::delFromColl(const int &row, const QString &url)
{
    std::vector<int> rowList;
    QStringList urlList;
    collUrlList.removeAll(url);
    rowList.push_back(row);
    urlList.append(url);
    delMetaData(rowList,urlList,"我的收藏");
}


void PlayerControl::createPlayList(const QString &playListName, const QString &path_Common, const QString &path_Hover)
{
    SelectPlayListButton *button = new SelectPlayListButton(playListName,path_Common, path_Hover);//初始化按钮，传入歌单名，以及正常态和悬浮态的图片
    QWidget *widget = new QWidget();
    widget->setFixedSize(160,40);
    button->setParent(widget);
    button->move((widget->width()-button->width())/2,(widget->height()-button->height())/2);
    button->setCheckable(true);
    connect(button,&SelectPlayListButton::playListChanged,this,&PlayerControl::onPlayListChanged);
    btnGroup_playList->addButton(button);
    if (db->createPlayListNotExist(playListName)) {
        QListWidgetItem *item = new QListWidgetItem;
        ui->listWidget_CreateMusic->addItem(item);
        item->setSizeHint(widget->size());
        ui->listWidget_CreateMusic->setItemWidget(item,widget);
    }
}


void PlayerControl::onBtnAddPlayListClicked()
{
    AddPlayListPop *newPop = new AddPlayListPop(this);
    connect(newPop,&AddPlayListPop::createPlayListRequest,this,&PlayerControl::createPlayList);
    newPop->show();
    //为主窗口添加模糊效果
    QGraphicsBlurEffect *blureffect = new QGraphicsBlurEffect;
    blureffect->setBlurRadius(3); //数值越大，越模糊
    setGraphicsEffect(blureffect);
}


void PlayerControl::updatePlayListInfo(int infoNums)
{
    while (infoNums>=0&&ui->listWidget_PlayListInfo->count()<itemWidgetList.count()) {
        QListWidgetItem *item = new QListWidgetItem();
        ui->listWidget_PlayListInfo->addItem(item);
        item->setSizeHint(itemWidgetList.at(ui->listWidget_PlayListInfo->count()-1)->size());
        ui->listWidget_PlayListInfo->setItemWidget(item,itemWidgetList.at(ui->listWidget_PlayListInfo->count()-1));
        infoNums--;
    }
}


void PlayerControl::onPlayListChanged(const QString &playListName)
{
    ui->stackedWidget_Body->setCurrentWidget(ui->page_Body_PlayListInfo);
    ui->widget_Tool->show();
    on_btn_QuitBatch_clicked();
    checkbox_SelectAll->setCheckState(Qt::CheckState::Unchecked);
    ui->label_PlayListName->setText(playListName);//接收由按钮发来的歌单名
    //歌单切换，清除已更新的和待更新的歌单列表信息
    itemWidgetList.clear();
    ui->listWidget_PlayListInfo->clear();
    emit workRead_Stop();//停止正在进行的读取任务
    //重新创建一个读取工作并加入线程池中
    WorkReadMetaData *readWork = new WorkReadMetaData;
    readWork->db = db->db;
    readWork->playListName = playListName;
    connect(readWork,&WorkReadMetaData::updateInfoRequest,this,&PlayerControl::onUpdateInfoRequest);
    connect(this,&PlayerControl::workRead_Stop,readWork,&WorkReadMetaData::stopWork);
    connect(readWork,&WorkReadMetaData::workFinished,this,&PlayerControl::onWorkReadFinished);
    QThreadPool::globalInstance()->start(readWork);
}


void PlayerControl::onUpdateInfoRequest(QString url, QString playListName, QPixmap poster, QString title,
                                        QString artist, QString album, QString duration, int collStatus)
{
    if (!collUrlList.isEmpty()&&playListName!="我的收藏") {
        if (collUrlList.contains(url)) {
            collStatus = 1;
        }
        else {
            collStatus = 0;
        }
    }
    if (infoHasBeenUpdated < 14) {
        MusicInfoListItem *itemWidget = new MusicInfoListItem(url,playListName,itemWidgetList.count(),
                                                              poster,title,artist,album,duration,collStatus);
        itemWidget->init_MusicInfoItem();
        connect(itemWidget,&MusicInfoListItem::addToCollRequest,this,&PlayerControl::addToColl);//添加到"我的收藏"
        connect(itemWidget,&MusicInfoListItem::delFromCollRequest,this,&PlayerControl::delFromColl);//从"我的收藏"删除
        connect(itemWidget,&MusicInfoListItem::sendDeleteRow,this,&PlayerControl::recDeleteRow);//实时接收需要删除的行号
        connect(this,&PlayerControl::batchEnterRequest,itemWidget,&MusicInfoListItem::batchEnter);//进入批量操作模式
        connect(this,&PlayerControl::batchQuitRequest,itemWidget,&MusicInfoListItem::batchQuit);//退出批量操作模式
        connect(checkbox_SelectAll,&MyCheckBox::checkStateChanged,itemWidget,&MusicInfoListItem::setNewBatchStatus);
        itemWidgetList.append(itemWidget);
        //直接更新歌曲信息
        QListWidgetItem *item = new QListWidgetItem();
        ui->listWidget_PlayListInfo->addItem(item);
        item->setSizeHint(itemWidget->size());
        ui->listWidget_PlayListInfo->setItemWidget(item,itemWidget);
        infoHasBeenUpdated++;
        return;
    }
    //加入待更新的歌曲信息列表(需要显示的数量达到阈值)
    MusicInfoListItem *itemWidget = new MusicInfoListItem(url,playListName,itemWidgetList.count(),
                                                          poster,title,artist,album,duration,collStatus);
    itemWidget->init_MusicInfoItem();
    connect(itemWidget,&MusicInfoListItem::addToCollRequest,this,&PlayerControl::addToColl);
    connect(itemWidget,&MusicInfoListItem::delFromCollRequest,this,&PlayerControl::delFromColl);
    connect(itemWidget,&MusicInfoListItem::sendDeleteRow,this,&PlayerControl::recDeleteRow);
    connect(this,&PlayerControl::batchEnterRequest,itemWidget,&MusicInfoListItem::batchEnter);
    connect(this,&PlayerControl::batchQuitRequest,itemWidget,&MusicInfoListItem::batchQuit);
    connect(checkbox_SelectAll,&MyCheckBox::checkStateChanged,itemWidget,&MusicInfoListItem::setNewBatchStatus);
    itemWidgetList.append(itemWidget);
}


void PlayerControl::onWorkReadFinished()
{
    infoHasBeenUpdated = 0;//读取工作结束时，重置需要立即更新的歌曲信息数
}

void PlayerControl::onSliderTimeRelease()
{
    myPlayer->setPosition(ui->horizontalSliderTime->value());
}

void PlayerControl::onBtnLastOneClicked()
{
    if (playlist->itemWidgetList.count()>0) {
        emit sendMediaRequest(playlist->getCurrentRow(),-1);
    }
}

void PlayerControl::onBtnPlayOrPauseClicked()
{
    if (myPlayer->playbackState() == QMediaPlayer::PlayingState) {
        myPlayer->pause();
        btn_PlayOrPause->setIcon(QIcon(":/icon/play.png"));
    }
    else if (myPlayer->playbackState() == QMediaPlayer::PausedState) {
        myPlayer->play();
        btn_PlayOrPause->setIcon(QIcon(":/icon/stop.png"));
    }
}

void PlayerControl::onBtnNextOneClicked()
{
    if (playlist->itemWidgetList.count()>0) {
        emit sendMediaRequest(playlist->getCurrentRow(),1);
    }
}

void PlayerControl::onTimerVolumeOut()
{
    times++;
    if (times>=2) {
        times=0;
        if (!widget_Volume->isHidden()) {
            widget_Volume->hide();
        }
        timer_Volume->stop();
    }

}

void PlayerControl::showPlayList(bool checked)
{
    if (checked) {
        playlist->move(width()-playlist->width(),60);
        playlist->setFixedHeight(ui->widget_Body->height());
        playlist->show();
    }
    else {
        playlist->hide();
    }
}


void PlayerControl::updateVisibleItems()
{
    if (itemWidgetList.count()>0)
    emit updatePlayListInfoRequest(8);
}

void PlayerControl::on_btn_ImportTool_clicked()
{
    QFileDialog filedialog;
    filedialog.setFileMode(QFileDialog::ExistingFiles);//文件对话框为打开类型
    //装载过滤器
    QStringList filters;
    filters << "Music (*.mp3 *.wav *.flac)";
    filedialog.setNameFilters(filters);
    QString playListName = ui->label_PlayListName->text();

    if (filedialog.exec() == QFileDialog::Accepted) {//用户选择文件完毕
        QStringList urlList = filedialog.selectedFiles();
        infoHasBeenUpdated = 0;
        saveMetaData(urlList,playListName,true);
    }
}


void PlayerControl::saveMetaData(const QStringList &urlList, const QString &playListName, bool updateStatus)
{
    //创建一个保存元数据的工作并加入线程池中
    WorkSaveMetaData *saveWork = new WorkSaveMetaData;
    saveWork->db = db;
    saveWork->playListName = playListName;
    saveWork->urlList = urlList;
    //信号与槽通信
    /*阻止DataBase对象继续在saveMetaData函数发送更新UI信息的请求*/
    connect(this,&PlayerControl::workSave_StopUpdate,saveWork,&WorkSaveMetaData::stopUpdate);
    /*停止保存工作*/
    connect(this,&PlayerControl::workSave_Stop,saveWork,&WorkSaveMetaData::stopWork);
    if (!updateStatus) {
        emit workSave_StopUpdate();
    }
    QThreadPool::globalInstance()->start(saveWork);
}

void PlayerControl::delMetaData(std::vector<int> rowList, const QStringList &urlList, const QString &playListName)
{
    std::sort(rowList.begin(), rowList.end(), [](int a, int b) { return a > b;}); // 使用 lambda 表达式进行降序排序
    QString playListName_Now = ui->label_PlayListName->text();
    //遍历行号，从后往前删除对应行的UI
    for (auto it = rowList.begin(); it != rowList.end(); it++) {
        if (playListName_Now == playListName)
        {
            int row = *it;
            itemWidgetList.remove(row);
            QListWidgetItem* item = ui->listWidget_PlayListInfo->takeItem(row);
            // if (!item) {
            //     qDebug() << "歌曲信息删除失败：" << row << "  From void PlayerControl::delMetaData(const QList<int> &rowList, "
            //                                                "const QStringList &urlList, const QString &playListName)";
            // }
        }
    }
    //遍历歌曲信息列表，更新行号
    for (int i = 0; i < ui->listWidget_PlayListInfo->count(); i++) {
        QListWidgetItem *item = ui->listWidget_PlayListInfo->item(i);
        MusicInfoListItem *itemWidget = static_cast<MusicInfoListItem*>(ui->listWidget_PlayListInfo->itemWidget(item));
        QLabel *label_row = itemWidget->findChild<QLabel*>("label_row");
        if (itemWidget) {
            itemWidget->setRow(i);
        }
        if (label_row) {
            if (i<9) {
                label_row->setText("0"+QString::number(i+1));
            }
            else {
                label_row->setText(QString::number(i+1));
            }
        }
    }
    //创建删除任务，从数据库中删除歌曲信息
    WorkDeleteMetaData *deleteWork = new WorkDeleteMetaData;
    deleteWork->db = db->db;
    deleteWork->urlList = urlList;
    deleteWork->playListName = playListName;
    QThreadPool::globalInstance()->start(deleteWork);
}

bool PlayerControl::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == btn_Volume) {
        switch (event->type()) {
        case QEvent::Enter:
            if (timer_Volume->isActive()) {
                timer_Volume->stop();
            }
            pos = btn_Volume->mapTo(this,btn_Volume->rect().topLeft()) - QPoint(20,widget_Volume->height()+6);
            widget_Volume->move(pos);
            if (widget_Volume->isHidden()) {
                widget_Volume->show();
            }
            break;
        case QEvent::Leave:
            if (!widget_Volume->isHidden()) {
                timer_Volume->start(100);
            }
            break;
        default:
            break;
        }
    }

    if (watched == widget_Volume) {
        switch (event->type()) {
        case QEvent::Enter:
            if (timer_Volume->isActive()) {
                timer_Volume->stop();
            }
            break;
        case QEvent::Leave:
            if (!widget_Volume->isHidden()) {
                timer_Volume->start(100);
            }
            break;
        default:
            break;
        }
    }
    return QWidget::eventFilter(watched,event);
}

void PlayerControl::on_btn_BatchTool_clicked()
{
    emit batchEnterRequest();
    ui->stackedWidget_Tool->setCurrentWidget(ui->page_BatchOperation);
    batchMode = BatchMode::CheckBoxMode;
    checkbox_SelectAll->setCheckState(Qt::Unchecked);
    if (ui->label_PlayListName->text() == "全部音乐") {
        ui->btn_DeleteTool->setDisabled(true);
    }
    else {
        ui->btn_DeleteTool->setDisabled(false);
    }
}


void PlayerControl::on_btn_QuitBatch_clicked()
{
    emit batchQuitRequest();
    ui->stackedWidget_Tool->setCurrentWidget(ui->page_BasicTool);
    batchMode = BatchMode::NumberMode;
    checkbox_SelectAll->setCheckState(Qt::Checked);
    checkbox_SelectAll->setIcon_Now(":/icon/rect_grey.png");
}


void PlayerControl::on_btn_DeleteTool_clicked()
{
    int deleteNums = 0;
    deleteNums = deleteRowList.size();
    if (deleteNums > 0) {
        QString warnText = QString("是否确认删除%1首歌曲？").arg(QString::number(deleteNums));
        WarnPop *newPop = new WarnPop(warnText,this);
        newPop->toDeletePlayListInfo = true;
        connect(newPop,&WarnPop::confirmToDeletePlayListInfo,this,&PlayerControl::deletePlayListInfo);
        newPop->show();
    }
}


void PlayerControl::on_listWidget_PlayListInfo_itemDoubleClicked(QListWidgetItem *item)
{
    int row;
    QString url;
    QString playListName;
    QPixmap poster;
    QString title;
    QString artist;
    QString album;
    QString duration;
    int collStatus;
    playlist->listWidget->clear();
    playlist->itemWidgetList.clear();
    int currentIndex = ui->listWidget_PlayListInfo->row(item)+playlist->itemWidgetList.count();
    for (int i = 0; i < itemWidgetList.count(); i++) {
        //获取歌曲信息列表中的元数据
        row = itemWidgetList.at(i)->row();
        url = itemWidgetList.at(i)->url();
        playListName = itemWidgetList.at(i)->playListName();
        poster = itemWidgetList.at(i)->poster();
        title = itemWidgetList.at(i)->title();
        artist = itemWidgetList.at(i)->artist();
        album = itemWidgetList.at(i)->album();
        duration = itemWidgetList.at(i)->duration();
        collStatus = itemWidgetList.at(i)->collStatus();
        //初始化一个歌曲信息UI
        MusicInfoListItem *itemWidget = new MusicInfoListItem(url,playListName,row,poster,title,artist,album,duration,collStatus);
        connect(itemWidget,&MusicInfoListItem::addToCollRequest,this,&PlayerControl::addToColl);
        //connect(itemWidget,&MusicInfoListItem::delFromCollRequest,this,&PlayerControl::delFromColl);
        connect(itemWidget,&MusicInfoListItem::sendDeleteRow_PlayList,this,&PlayerControl::recDeleteRow_PlayList);
        itemWidget->setMaximumWidth(420);
        QListWidgetItem *newItem  = new QListWidgetItem;
        itemWidget->init_PlayListInfoItem();//初始化播放列表部件
        newItem->setSizeHint(itemWidget->size());
        playlist->itemWidgetList.append(itemWidget);//播放队列加入该歌曲信息
        playlist->listWidget->addItem(newItem);
        playlist->listWidget->setItemWidget(newItem,itemWidget);
    }
    playlist->setLabel_Nums(playlist->itemWidgetList.count());
    emit sendMediaRequest(currentIndex,0);//请求下标为currentIndex的媒体
}

void PlayerControl::on_btn_PlayTool_clicked()
{
    int row;
    QString url;
    QString playListName;
    QPixmap poster;
    QString title;
    QString artist;
    QString album;
    QString duration;
    int collStatus;
    int c = playlist->itemWidgetList.count();
    for (int i = 0; i < itemWidgetList.count(); i++) {
        //获取歌曲信息列表中的元数据
        row = itemWidgetList.at(i)->row()+c;
        url = itemWidgetList.at(i)->url();
        playListName = itemWidgetList.at(i)->playListName();
        poster = itemWidgetList.at(i)->poster();
        title = itemWidgetList.at(i)->title();
        artist = itemWidgetList.at(i)->artist();
        album = itemWidgetList.at(i)->album();
        duration = itemWidgetList.at(i)->duration();
        collStatus = itemWidgetList.at(i)->collStatus();
        //初始化一个歌曲信息UI
        MusicInfoListItem *itemWidget = new MusicInfoListItem(url,playListName,row,poster,title,artist,album,duration,collStatus);
        connect(itemWidget,&MusicInfoListItem::addToCollRequest,this,&PlayerControl::addToColl);
        //connect(itemWidget,&MusicInfoListItem::delFromCollRequest,this,&PlayerControl::delFromColl);
        connect(itemWidget,&MusicInfoListItem::sendDeleteRow_PlayList,this,&PlayerControl::recDeleteRow_PlayList);
        itemWidget->setMaximumWidth(420);
        QListWidgetItem *newItem  = new QListWidgetItem;
        itemWidget->init_PlayListInfoItem();//初始化播放列表部件
        newItem->setSizeHint(itemWidget->size());
        playlist->itemWidgetList.append(itemWidget);//播放队列加入该歌曲信息
        playlist->listWidget->addItem(newItem);
        playlist->listWidget->setItemWidget(newItem,itemWidget);
    }
    playlist->setLabel_Nums(playlist->itemWidgetList.count());
}


void PlayerControl::on_btn_Logo_clicked()
{
    ui->stackedWidget_Body->setCurrentWidget(ui->page_Lyrics);
}


void PlayerControl::on_lineEdit_Search_textChanged(const QString &arg1)
{
    if (!arg1.isEmpty()) {
        tempList.clear();
        for (int i = 0; i < itemWidgetList.count(); i++) {
            if (itemWidgetList.at(i)->title().contains(arg1) ||
                itemWidgetList.at(i)->artist().contains(arg1) ||
                itemWidgetList.at(i)->album().contains(arg1))
            {
                int row;
                QString url;
                QString playListName;
                QPixmap poster;
                QString title;
                QString artist;
                QString album;
                QString duration;
                int collStatus;
                row = i;
                url = itemWidgetList.at(i)->url();
                playListName = itemWidgetList.at(i)->playListName();
                poster = itemWidgetList.at(i)->poster();
                title = itemWidgetList.at(i)->title();
                artist = itemWidgetList.at(i)->artist();
                album = itemWidgetList.at(i)->album();
                duration = itemWidgetList.at(i)->duration();
                collStatus = itemWidgetList.at(i)->collStatus();
                MusicInfoListItem *itemWidget = new MusicInfoListItem(url,playListName,row,poster,title,artist,album,duration,collStatus);
                connect(itemWidget,&MusicInfoListItem::addToCollRequest,this,&PlayerControl::addToColl);
                connect(itemWidget,&MusicInfoListItem::delFromCollRequest,this,&PlayerControl::delFromColl);
                connect(itemWidget,&MusicInfoListItem::sendDeleteRow,this,&PlayerControl::recDeleteRow);
                connect(this,&PlayerControl::batchEnterRequest,itemWidget,&MusicInfoListItem::batchEnter);
                connect(this,&PlayerControl::batchQuitRequest,itemWidget,&MusicInfoListItem::batchQuit);
                connect(checkbox_SelectAll,&MyCheckBox::checkStateChanged,itemWidget,&MusicInfoListItem::setNewBatchStatus);
                itemWidget->init_MusicInfoItem();
                tempList.append(itemWidget);
            }
        }
        itemWidgetList.clear();
        ui->listWidget_PlayListInfo->clear();
        for (int i = 0; i < tempList.count(); i++) {
            QListWidgetItem *item = new QListWidgetItem;
            item->setSizeHint(tempList.at(i)->size());
            ui->listWidget_PlayListInfo->addItem(item);
            ui->listWidget_PlayListInfo->setItemWidget(item,tempList.at(i));
        }
    }
    else {
        onPlayListChanged(ui->label_PlayListName->text());
    }
}


void PlayerControl::on_btn_SearchTool_clicked()
{
    if (ui->lineEdit_Search->width() <= 0) {
        timer_Search2->start(3);
    }
    else {
        timer_Search->start(3);
    }
}

