#include "collectbutton.h"

CollectButton::CollectButton(int status)
{
    collStatus = status;
    setStyleSheet("background:transparent;border:none;");
    if (collStatus == 1) {
        setIcon(QIcon(":/icon/mylove3.png"));
    } else {
        setIcon(QIcon(":/icon/mylove_dark.png"));
    }
    setIconSize(QSize(16,16));
    setCursor(Qt::PointingHandCursor);
    setFlat(true);
    InitTimer();
}

void CollectButton::InitTimer()
{
    QSize beginSize = iconSize();
    QSize endSize = QSize(22,22);
    connect(&timer_IntervalTime,&QTimer::timeout,[=](){
        if (times == 1) {
            times = 0;
            timer_Magnify.start(10);
            timer_IntervalTime.stop();
        } else {
            times++;
        }
    });
    connect(&timer_Magnify,&QTimer::timeout,[=](){
        setIconSize(QSize(iconSize().width()+1,iconSize().height()+1));
        if (iconSize() == endSize) {
            timer_Magnify.stop();
            timer_Shrink.start(10);
        }
    });
    connect(&timer_Shrink,&QTimer::timeout,[=](){
        setIconSize(QSize(iconSize().width()-1,iconSize().height()-1));
        if (iconSize() == beginSize) {
            timer_Shrink.stop();
            times++;
            if (times<2) {
                timer_Magnify.start(10);
            } else {
                times = 0;
            }
        }
    });
}

bool CollectButton::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::Enter:
        if (collStatus == 0) {
            setIcon(QIcon(":/icon/mylove2.png"));
        }
        break;
    case QEvent::Leave:
        if (collStatus == 0) {
            setIcon(QIcon(":/icon/mylove_dark.png"));
        }
        break;
    case QEvent::MouseButtonPress:
        if (collStatus == 0) {
            setIcon(QIcon(":/icon/mylove3.png"));
            timer_IntervalTime.start(50);
            collStatus = 1;
            emit collStatusChg(1);
        } else {
            setIcon(QIcon(":/icon/mylove2.png"));
            collStatus = 0;
            emit collStatusChg(0);
        }
        break;
    default:
        break;
    }
    return QPushButton::event(e);
}
