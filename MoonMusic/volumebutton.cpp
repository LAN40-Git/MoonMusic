#include "volumebutton.h"

VolumeButton::VolumeButton(QWidget *parent)
{
    icon_common = icon_mute_common;
    icon_hover = icon_mute_hover;
    setIcon(QIcon(icon_common));
    setCursor(Qt::PointingHandCursor);
    setIconSize(QSize(18,18));
    setFixedSize(20,20);
    setStyleSheet("border:none;");
}

VolumeButton::~VolumeButton()
{
}

void VolumeButton::recSliderValue(int value)
{
    if (value == 0) {
        icon_common = icon_mute_common;
        icon_hover  = icon_mute_hover;
        setIcon(QIcon(icon_common));
    }
    else if (value >= 1 && value <= 30){
        icon_common = icon_low_common;
        icon_hover = icon_low_hover;
        setIcon(QIcon(icon_common));
    }
    else {
        icon_common = icon_high_common;
        icon_hover = icon_high_hover;
        setIcon(QIcon(icon_common));
    }
}

bool VolumeButton::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::Enter:
        setIcon(QIcon(icon_hover));
        break;
    case QEvent::Leave:
        setIcon(QIcon(icon_common));
        break;
    default:
        break;
    }
    return QPushButton::event(e);
}

