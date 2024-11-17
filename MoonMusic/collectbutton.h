#ifndef COLLECTBUTTON_H
#define COLLECTBUTTON_H

#include <QPushButton>
#include <QEvent>
#include <QPainter>
#include <QIcon>
#include <QTimer>

class CollectButton : public QPushButton
{
    Q_OBJECT
private:
    int collStatus = 0;//收藏状态:0表示未收藏
    QTimer timer_IntervalTime; //间隔时间
    QTimer timer_Magnify;
    QTimer timer_Shrink;
    int times = 0;

signals:
    void collStatusChg(const int &newCollStatus);
public:
    CollectButton(int status);
    void InitTimer();
private:
    bool event(QEvent *e) override;
};

#endif // COLLECTBUTTON_H
