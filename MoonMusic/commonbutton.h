#ifndef COMMONBUTTON_H
#define COMMONBUTTON_H

#include <QPushButton>
#include <QEvent>
#include <QMouseEvent>
#include <QString>
#include <QIcon>
#include <QSize>

class CommonButton : public QPushButton
{
    Q_OBJECT
    QString m_icon_Common; //鼠标正常图标
    QString m_icon_Hover;  //鼠标悬浮图标
    QString m_icon_Pressed;//鼠标按下图标

public:
    CommonButton(const QString &icon_Common, const QString &icon_Hover, const QString &icon_Pressed, const QSize &icon_size, const QSize &btn_size);

private:
    bool event(QEvent *e) override;
};

#endif // COMMONBUTTON_H
