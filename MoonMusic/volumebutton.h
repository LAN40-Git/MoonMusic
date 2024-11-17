#ifndef VOLUMEBUTTON_H
#define VOLUMEBUTTON_H

#include <QPushButton>
#include <QEvent>
#include <QWidget>
#include <QSlider>
#include "volumewidget.h"

class VolumeButton : public QPushButton
{
    Q_OBJECT
    QString icon_common;
    QString icon_hover;

    //静音图标
    QString icon_mute_common = ":/icon/volume_mute.png";
    QString icon_mute_hover = ":/icon/volume_mute_hover.png";

    //一级音量图标
    QString icon_low_common = ":/icon/volume_low.png";
    QString icon_low_hover = ":/icon/volume_low_hover.png";

    //二级音量图标
    QString icon_high_common = ":/icon/volume_high.png";
    QString icon_high_hover = ":/icon/volume_high_hover.png";

    VolumeWidget *widget_Volume;

public:
    explicit VolumeButton(QWidget *parent = nullptr);
    ~VolumeButton();

public slots:
    void recSliderValue(int value);

private:
    bool event(QEvent *e) override;
};

#endif // VOLUMEBUTTON_H
