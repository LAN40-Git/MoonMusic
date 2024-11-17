#include "playlist.h"
#include "ui_playlist.h"

#include <QGraphicsDropShadowEffect>
#include <QRandomGenerator>

int PlayList::getCurrentRow() const
{
    return currentRow;
}

void PlayList::setCurrentRow(int newCurrentRow)
{
    currentRow = newCurrentRow;
}

PlayMode PlayList::getCurrentPlayMode() const
{
    return currentPlayMode;
}

void PlayList::setCurrentPlayMode(PlayMode newCurrentPlayMode)
{
    currentPlayMode = newCurrentPlayMode;
}

PlayList::PlayList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlayList)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint);

    ui->lineEdit->setFixedHeight(0);

    btn_Clear = new CommonButton(":/icon/delete.png",":/icon/delete2.png",":/icon/delete3.png",QSize(20,20),QSize(30,30));
    btn_Clear->setToolTip("清除播放队列");
    btn_Clear->setParent(ui->widget);
    btn_Clear->move(340,5);
    connect(btn_Clear,&CommonButton::clicked,this,&PlayList::btnClearClicked);

    btn_Search = new CommonButton(":/icon/playlist_search.png",":/icon/playlist_search_hover.png",
                                  ":/icon/playlist_search_pressed.png",QSize(17,17),QSize(30,30));
    btn_Search->setToolTip("搜索");
    btn_Search->setParent(ui->widget);
    btn_Search->move(300,6);
    btn_Search->setCheckable(true);
    connect(btn_Search,&CommonButton::clicked,this,&PlayList::btnSearchClicked);

    listWidget = new QListWidget(this);
    listWidget->setMinimumSize(420,480);
    listWidget->setMaximumWidth(420);
    listWidget->setStyleSheet("border:none;border-radius:0;");
    ui->verticalLayout->addWidget(listWidget);
    listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);//实例阴影shadow
    shadow->setOffset(0, 0);//设置阴影距离
    shadow->setColor(QColor("#444444"));//设置阴影颜色
    shadow->setBlurRadius(30);//设置阴影圆角
    ui->lineEdit->setGraphicsEffect(shadow);//给嵌套QWidget设置阴影
    ui->lineEdit->setContentsMargins(1,0,0,0);
}

PlayList::~PlayList()
{
    delete ui;
}

void PlayList::setLabel_Nums(int newNums)
{
    QString text = QString("共%1首").arg(QString::number(newNums));
    ui->label_Nums->setText(text);
}

void PlayList::deleteRow(int row)
{
    QListWidgetItem *item = listWidget->takeItem(row);
    delete item;
    item = nullptr;
    for (int i = 0; i < listWidget->count(); i++) {
        QListWidgetItem *item_i = listWidget->item(i);
        MusicInfoListItem* itemWidget = static_cast<MusicInfoListItem*>(listWidget->itemWidget(item_i));
        itemWidget->setRow(i);
        QLabel* label_row = itemWidget->findChild<QLabel*>("label_row");
        if (label_row) {
            if (i<9) {
                label_row->setText("0"+QString::number(i+1));
            }
            else {
                label_row->setText(QString::number(i+1));
            }
        }
    }
    itemWidgetList.remove(row);
    setLabel_Nums(itemWidgetList.count());
}

void PlayList::recPlayMode(PlayMode mode)
{
    currentPlayMode = mode;
}

void PlayList::recMediaRequest(int index, int state)
{
    switch (state) {
    case -1://上一首的媒体
        switch (currentPlayMode) {
        case PlayMode::SequenceMode:
            getLastMedia(index);
            break;
        case PlayMode::SingleLoopMode:
            getLastMedia(index);
            break;
        case PlayMode::RandomMode:
            getRandomMedia();
            break;
        default:
            break;
        }
        break;
    case 0://下标为index的媒体
        getIndexMedia(index);
        break;
    case 1://下一首的媒体
        switch (currentPlayMode) {
        case PlayMode::SequenceMode:
            getNextMedia(index);
            break;
        case PlayMode::SingleLoopMode:
            getNextMedia(index);
            break;
        case PlayMode::RandomMode:
            getRandomMedia();
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}


void PlayList::btnSearchClicked(bool checked)
{
    if (checked) {
        ui->lineEdit->setFixedHeight(30);
    }
    else {
        ui->lineEdit->setFixedHeight(0);
        ui->lineEdit->clear();
    }

}

void PlayList::btnClearClicked()
{
    listWidget->clear();
    itemWidgetList.clear();
    setLabel_Nums(itemWidgetList.count());
}

void PlayList::getLastMedia(int index)
{
    if (index == 0) {
        currentRow = itemWidgetList.count()-1;
        emit sendMedia(itemWidgetList.at(currentRow));
    }
    else {
        if (index > 0 && index < itemWidgetList.count()) {
            currentRow = index-1;
            emit sendMedia(itemWidgetList.at(currentRow));
        }
    }
}

void PlayList::getIndexMedia(int index)
{
    currentRow =index;
    if (index < itemWidgetList.count() && index >= 0) {
        emit sendMedia(itemWidgetList.at(currentRow));
    }
}

void PlayList::getNextMedia(int index)
{
    if (index == itemWidgetList.count()-1) {
        currentRow = 0;
        emit sendMedia(itemWidgetList.at(currentRow));
    }
    else {
        if (index >= 0 && index < itemWidgetList.count()-1) {
            currentRow = index+1;
            emit sendMedia(itemWidgetList.at(currentRow));
        }
    }
}

void PlayList::getRandomMedia()
{
    int randomValue;
    randomValue = QRandomGenerator::global()->bounded(itemWidgetList.count());
    if (randomValue >= 0 && randomValue < itemWidgetList.count()) {
        currentRow = randomValue;
        emit sendMedia(itemWidgetList.at(currentRow));
    }
}
