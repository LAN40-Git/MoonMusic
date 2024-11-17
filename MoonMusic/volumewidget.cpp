#include "volumewidget.h"

#include <QFile>
#include <QGraphicsDropShadowEffect>

VolumeWidget::VolumeWidget(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(60,212);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("border:none;");
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);//实例阴影shadow
    shadow->setOffset(0, 0);//设置阴影距离
    shadow->setColor(QColor("#444444"));//设置阴影颜色
    shadow->setBlurRadius(20);//设置阴影圆角
    setGraphicsEffect(shadow);
    setContentsMargins(1,1,1,1);
}

void VolumeWidget::init_Slider(int value)
{
    v_slider = new QSlider(this);
    v_slider->setFixedSize(60,160);
    v_slider->move(0,10);
    v_slider->setMaximum(100);
    v_slider->setValue(value);
    QFile file(":/style/slider_style.css");
    if (file.open(QIODevice::ReadOnly)) {
        QString text = file.readAll();
        v_slider->setStyleSheet(text);
    }
    connect(v_slider,&QSlider::valueChanged,this,&VolumeWidget::onSliderValueChg);
}

void VolumeWidget::init_Label()
{
    label_percent = new QLabel(this);
    label_percent->setFixedSize(60,20);
    label_percent->move(0,170);
    label_percent->setStyleSheet("color:rgb(170,170,170);padding:0 0 5 18;font:微软雅黑 10pt;");
    label_percent->setText(QString::number(v_slider->value())+"%");
}

void VolumeWidget::onSliderValueChg(int value)
{
    label_percent->setText(QString::number(value)+"%");
    sendSliderValue(value);
}

void VolumeWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);//文字抗锯齿

    painter.setBrush(QColor(255,255,255));
    painter.drawRoundedRect(rect().x(),rect().y(),60,200,8,8);

    QPainterPath path;
    path.moveTo(rect().x()+20,rect().bottom()-12);
    path.lineTo(rect().x()+30,rect().bottom());
    path.lineTo(rect().x()+40,rect().bottom()-12);
    painter.drawPath(path);


}
