#ifndef WARNPOP_H
#define WARNPOP_H

#include "commonbutton.h"

#include <QWidget>

namespace Ui {
class WarnPop;
}

class WarnPop : public QWidget
{
    Q_OBJECT

public:
    explicit WarnPop(QString text, QWidget *parent = nullptr);
    ~WarnPop();
    bool toDeletePlayListInfo = false;//点击确认时是否发送删除歌曲信息的信号
signals:

    void confirmToDeletePlayListInfo();//确认删除歌单歌曲信息的信号

private slots:
    void on_btn_Cancle_clicked();

    void on_btn_Confirm_clicked();

private:
    Ui::WarnPop *ui;

    CommonButton *btn_Close;

    void onBtnCloseClicked();

    //根据需要显示的信息计算出警告框的舒适宽度
    void setSuitableWidthByText(QString text);
};

#endif // WARNPOP_H
