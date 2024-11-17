#ifndef ADDPLAYLISTPOP_H
#define ADDPLAYLISTPOP_H

#include <QWidget>

#include "commonbutton.h"

namespace Ui {
class AddPlayListPop;
}

class AddPlayListPop : public QWidget
{
    Q_OBJECT

public:
    explicit AddPlayListPop(QWidget *parent = nullptr);
    ~AddPlayListPop();

public slots:
    void onBtnCloseClicked();

private slots:
    void on_btn_Cancle_clicked();

    void on_btn_Confirm_clicked();

    void on_lineEdit_cursorPositionChanged(int arg1, int arg2);

signals:
    void createPlayListRequest(const QString &playListName, const QString &path_Common, const QString &path_Hover);
private:
    Ui::AddPlayListPop *ui;
    CommonButton *btn_Close;
};

#endif // ADDPLAYLISTPOP_H
