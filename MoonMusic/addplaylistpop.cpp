#include "addplaylistpop.h"
#include "ui_addplaylistpop.h"
#include "playercontrol.h"

AddPlayListPop::AddPlayListPop(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddPlayListPop)
{
    ui->setupUi(this);

    ui->label_Warn->hide();
    ui->label_Warn_2->hide();
    //输入文本为空时禁用创建按钮
    ui->btn_Confirm->setDisabled(true);
    //初始化关闭按钮
    btn_Close = new CommonButton(":/icon/close_grey.png",":/icon/close_hover.png",":/icon/close_pressed.png",QSize(13,12),QSize(30,30));
    btn_Close->setParent(ui->widget_All);
    btn_Close->move(ui->widget_All->width()-38,8);
    connect(btn_Close,&CommonButton::clicked,this,&AddPlayListPop::onBtnCloseClicked);

    setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);
    setWindowModality(Qt::ApplicationModal); //设置窗口模态（只响应自己的输入事件）
}

AddPlayListPop::~AddPlayListPop()
{
    delete ui;
}

void AddPlayListPop::onBtnCloseClicked()
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

void AddPlayListPop::on_btn_Cancle_clicked()
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


void AddPlayListPop::on_btn_Confirm_clicked()
{
    QString playListName = ui->lineEdit->text();
    QChar ch = ' ';
    if (!playListName.contains(ch)) {//如果去除空格之后的字符串不存在空格
        //发送创建歌单的信号
        emit createPlayListRequest(playListName,
                                   ":/icon/playlist.png",
                                   ":/icon/playlist2.png");
        onBtnCloseClicked();
    } else {//如果存在，则提示
        ui->label_Warn->show();
    }
}

void AddPlayListPop::on_lineEdit_cursorPositionChanged(int arg1, int arg2)
{
    if (!ui->label_Warn->isHidden()) {
        ui->label_Warn->hide();
    }
    if (!ui->label_Warn_2->isHidden()) {
        ui->label_Warn_2->hide();
    }
    if (arg2 == 30) {
        ui->label_Warn_2->show();
    }
    if (arg2 == 0) {
        ui->btn_Confirm->setDisabled(true);
    } else {
        ui->btn_Confirm->setDisabled(false);
    }
}

