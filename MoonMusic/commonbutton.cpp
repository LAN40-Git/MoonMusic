#include "commonbutton.h"

CommonButton::CommonButton(const QString &icon_Common, const QString &icon_Hover, const QString &icon_Pressed, const QSize &icon_size, const QSize &btn_size)
{
    setFixedSize(btn_size);
    setIconSize(icon_size);
    setIcon(QIcon(icon_Common));
    setStyleSheet("background:transparent;border:none;");
    setCursor(Qt::PointingHandCursor);
    m_icon_Common = icon_Common;
    m_icon_Hover = icon_Hover;
    m_icon_Pressed = icon_Pressed;
}

bool CommonButton::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::Enter:
        setIcon(QIcon(m_icon_Hover));
        break;
    case QEvent::Leave:
        setIcon(QIcon(m_icon_Common));
        break;
    case QEvent::MouseButtonPress:
        setIcon(QIcon(m_icon_Pressed));
        break;
    case QEvent::MouseButtonRelease:
        setIcon(QIcon(m_icon_Hover));
        break;
    default:
        break;
    }
    return QPushButton::event(e);
}
