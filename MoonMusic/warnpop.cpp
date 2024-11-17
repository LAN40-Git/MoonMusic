#include "warnpop.h"
#include "ui_warnpop.h"
#include "playercontrol.h"

#include <QChar>

WarnPop::WarnPop(QString text, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WarnPop)
{
    ui->setupUi(this);
    setSuitableWidthByText(text);
    //初始化关闭按钮
    btn_Close = new CommonButton(":/icon/close_grey.png",":/icon/close_hover.png",":/icon/close_pressed.png",QSize(13,12),QSize(30,30));
    btn_Close->setParent(ui->widget);
    btn_Close->move(width()-35,5);
    connect(btn_Close,&CommonButton::clicked,this,&WarnPop::onBtnCloseClicked);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);
    setWindowModality(Qt::ApplicationModal); //设置窗口模态（只响应自己的输入事件）
}

WarnPop::~WarnPop()
{
    delete ui;
}

void WarnPop::on_btn_Cancle_clicked()
{
    onBtnCloseClicked();
}

void WarnPop::onBtnCloseClicked()
{
    //为主窗口解除模糊效果
    PlayerControl *player = qobject_cast<PlayerControl*>(parent());
    if (player) {
        // 如果转换成功，则移除模糊效果
        player->setGraphicsEffect(nullptr);
    } else {
        qWarning() << "Parent window is not of type PlayerControl";
    }
    this->close();//关闭该窗口
}

void WarnPop::setSuitableWidthByText(QString text)
{
    int minWidth = 350;//警告框最小宽度
    int maxWidth = 720;
    int suitableWidth = 100;
    for (QChar ch : text) {
        if (ch.unicode() >= 0x4E00 && ch.unicode() <= 0x9FFF) {
            suitableWidth+=23;
        } else {
            suitableWidth+=12;
        }
    }
    if (suitableWidth > minWidth) {
        if (suitableWidth >= maxWidth) {
            setFixedWidth(maxWidth);
            QString exc = ui->label_WarnText->fontMetrics().elidedText(text,Qt::ElideRight,ui->label_WarnText->width()+220);
            ui->label_WarnText->setText(exc);
        } else {
            setFixedWidth(suitableWidth);
            ui->label_WarnText->setText(text);
        }
    } else {
        setFixedWidth(minWidth);
        ui->label_WarnText->setText(text);
    }
}

void WarnPop::on_btn_Confirm_clicked()
{
    if (toDeletePlayListInfo) {
        emit confirmToDeletePlayListInfo();
    }
    onBtnCloseClicked();
}

