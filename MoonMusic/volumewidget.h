#ifndef VOLUMEWIDGET_H
#define VOLUMEWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>

class VolumeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VolumeWidget(QWidget *parent = nullptr);
    QSlider *v_slider;
    QLabel *label_percent;//音量百分比

public:
    void init_Slider(int value);

    void init_Label();

private slots:
    void onSliderValueChg(int value);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void sendSliderValue(int value);
};

#endif // VOLUMEWIDGET_H
