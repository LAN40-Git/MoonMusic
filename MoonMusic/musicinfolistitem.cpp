#include "musicinfolistitem.h"

void MusicInfoListItem::recDeleteRow(bool status)
{
    emit sendDeleteRow(m_row,m_url,status);
}

void MusicInfoListItem::onCollStatusChg(const int &newCollStatus)
{
    switch (newCollStatus) {
    case 0:
        emit delFromCollRequest(m_row, m_url);
        break;
    case 1:
        emit addToCollRequest(m_url);
        break;
    default:
        break;
    }
}

void MusicInfoListItem::batchEnter()
{
    label_row->hide();
    checkbox->show();
}

void MusicInfoListItem::batchQuit()
{
    label_row->show();
    checkbox->hide();
}

int MusicInfoListItem::row() const
{
    return m_row;
}

void MusicInfoListItem::setRow(int newRow)
{
    m_row = newRow;
}

QString MusicInfoListItem::playListName() const
{
    return m_playListName;
}

void MusicInfoListItem::setPlayListName(const QString &newPlayListName)
{
    m_playListName = newPlayListName;
}

QPixmap MusicInfoListItem::poster() const
{
    return m_poster;
}

void MusicInfoListItem::setPoster(const QPixmap &newPoster)
{
    m_poster = newPoster;
}

QString MusicInfoListItem::title() const
{
    return m_title;
}

void MusicInfoListItem::setTitle(const QString &newTitle)
{
    m_title = newTitle;
}

QString MusicInfoListItem::artist() const
{
    return m_artist;
}

void MusicInfoListItem::setArtist(const QString &newArtist)
{
    m_artist = newArtist;
}

QString MusicInfoListItem::album() const
{
    return m_album;
}

void MusicInfoListItem::setAlbum(const QString &newAlbum)
{
    m_album = newAlbum;
}

QString MusicInfoListItem::duration() const
{
    return m_duration;
}

void MusicInfoListItem::setDuration(const QString &newDuration)
{
    m_duration = newDuration;
}

int MusicInfoListItem::collStatus() const
{
    return m_collStatus;
}

void MusicInfoListItem::setCollStatus(int newCollStatus)
{
    m_collStatus = newCollStatus;
}

QString MusicInfoListItem::url() const
{
    return m_url;
}

void MusicInfoListItem::setUrl(const QString &newUrl)
{
    m_url = newUrl;
}

void MusicInfoListItem::onBtnDeleteClicked()
{
    emit sendDeleteRow_PlayList(m_row);
}

void MusicInfoListItem::init_CheckBox(const int &index)
{
    checkbox = new MyCheckBox();
    connect(checkbox,&MyCheckBox::sendDeleteRow,this,&MusicInfoListItem::recDeleteRow);
    checkbox->setObjectName("checkbox");
    h_layout->addWidget(checkbox);
    checkbox->hide();
    h_layout->setStretch(index,30);
}

void MusicInfoListItem::init_Label_Row(const int &index)
{
    QString exc;

    label_row = new QLabel();
    label_row->setObjectName("label_row");
    label_row->setStyleSheet("font:9pt 微软雅黑;color:#7a7a7d;text-align:center;background:transparent;");
    h_layout->addWidget(label_row);
    h_layout->setStretch(index,20);
    label_row->setMinimumWidth(40);
    exc = label_row->fontMetrics().elidedText(QString::number(m_row+1),Qt::ElideRight,label_row->width());
    if (m_row<9) { exc = "0"+exc;}
    label_row->setText(exc);
    label_row->setWhatsThis(QString::number(m_row));
}

void MusicInfoListItem::init_Label_Poster(const int &index)
{
    label_poster = new QLabel();
    label_poster->setObjectName("label_poster");
    h_layout->addWidget(label_poster);
    h_layout->setStretch(index,60);
    label_poster->setFixedSize(50,50);
    label_poster->setPixmap(m_poster);
    label_poster->setScaledContents(true);
}

void MusicInfoListItem::init_Label_TitleAndLabel_Artist(const int &index)
{
    QString exc;

    label_title = new QLabel();
    label_title->setObjectName("label_title");
    label_title->setStyleSheet("font:14px 微软雅黑;color:#222222;background:transparent;");
    label_title->setMinimumWidth(70);


    label_artist = new QLabel();
    label_artist->setStyleSheet("font:9pt 微软雅黑;color:#7a7a7d;background:transparent;");
    label_artist->setMinimumWidth(70);
    label_artist->setObjectName("label_artist");

    exc = label_title->fontMetrics().elidedText(m_title,Qt::ElideRight,80);
    label_title->setText(exc);
    label_title->setWhatsThis(m_title);

    exc = label_artist->fontMetrics().elidedText(m_artist,Qt::ElideRight,80);
    label_artist->setText(exc);
    label_artist->setWhatsThis(m_artist);

    v_layout->addWidget(label_title);
    v_layout->addWidget(label_artist);
    v_layout->setContentsMargins(10,5,0,5);
    v_layout->setSpacing(0);
    v_layout->setStretch(0,25);
    v_layout->setStretch(1,25);
    h_layout->addLayout(v_layout);
    h_layout->setStretch(index,490);
}

void MusicInfoListItem::init_Label_Album(const int &index)
{
    QString exc;

    label_album = new QLabel();
    label_album->setObjectName("label_album");
    label_album->setStyleSheet("font: 14px 微软雅黑;color:#222222;background:transparent;");
    h_layout->addWidget(label_album);
    h_layout->setStretch(index,300);
    exc = label_album->fontMetrics().elidedText(m_album,Qt::ElideRight,80);
    label_album->setText(exc);
    label_album->setWhatsThis(m_album);
    label_album->setMinimumWidth(70);
}

void MusicInfoListItem::init_Label_Duration(const int &index)
{
    int64_t drtTime; //计算分秒显示的时长
    drtTime = m_duration.toInt();
    int sec = drtTime/1000000;
    int min = sec/60;
    sec %= 60;
    QString p_duration = QString("%1%2:%3%4").arg(QString::number(min/10),QString::number(min%10),
                                                  QString::number(sec/10),QString::number(sec%10));

    label_duration = new QLabel(p_duration);
    label_duration->setObjectName("label_duration");
    label_duration->setStyleSheet("font:10pt 微软雅黑;color:#7a7a7d;background:transparent;");
    h_layout->addWidget(label_duration);
    h_layout->setStretch(index,60);
    label_duration->setMinimumWidth(50);
}

void MusicInfoListItem::init_Btn_Collect(const int &index)
{
    btn_collect = new CollectButton(m_collStatus);
    connect(btn_collect,&CollectButton::collStatusChg,this,&MusicInfoListItem::onCollStatusChg);
    btn_collect->setObjectName("btn_collect");
    h_layout->addWidget(btn_collect);
    h_layout->setStretch(index,20);
    btn_collect->setFixedSize(30,30);
}

void MusicInfoListItem::init_Btn_Delete(const int &index)
{
    btn_delete = new CommonButton(":/icon/close_grey.png",":/icon/close_hover.png",":/icon/close_pressed.png",QSize(13,12),QSize(30,30));
    h_layout->addWidget(btn_delete);
    h_layout->setStretch(index,20);
    connect(btn_delete,&CommonButton::clicked,this,&MusicInfoListItem::onBtnDeleteClicked);
}

void MusicInfoListItem::init_MusicInfoItem()
{
    init_CheckBox(0);
    init_Label_Row(1);
    init_Label_Poster(2);
    init_Label_TitleAndLabel_Artist(3);
    init_Label_Album(4);
    init_Label_Duration(5);
    init_Btn_Collect(6);
    h_layout->setContentsMargins(10,0,30,0);
}

void MusicInfoListItem::init_PlayListInfoItem()
{
    init_Label_Row(0);
    init_Label_Poster(1);
    init_Label_TitleAndLabel_Artist(2);
    init_Label_Album(3);
    init_Label_Duration(4);
    init_Btn_Collect(5);
    init_Btn_Delete(6);
    h_layout->setContentsMargins(10,0,10,0);
}

MusicInfoListItem::MusicInfoListItem(const QString &url, const QString &playListName, int row, const QPixmap &poster, const QString &title,
                                     QString &artist, const QString &album, const QString &duration, const int &collStatus)
{
    m_row = row;
    m_url = url;
    m_playListName = playListName;
    m_poster = poster;
    m_title = title;
    m_artist = artist;
    m_album = album;
    m_duration = duration;
    m_collStatus = collStatus;
    h_layout = new QHBoxLayout();
    v_layout = new QVBoxLayout();
    this->setMaximumHeight(60);
    this->setLayout(h_layout);
}

void MusicInfoListItem::setNewBatchStatus(Qt::CheckState newstate)
{
    checkbox->setCheckState(newstate);
}
