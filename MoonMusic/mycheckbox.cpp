#include "mycheckbox.h"


QString MyCheckBox::getIcon_Now() const
{
    return icon_Now;
}

void MyCheckBox::setIcon_Now(const QString &newIcon_Now)
{
    icon_Now = newIcon_Now;
}


MyCheckBox::~MyCheckBox()
{

}


MyCheckBox::MyCheckBox()
{
    setFixedSize(QSize(13,13));
    setCursor(Qt::PointingHandCursor);
    connect(this,&MyCheckBox::checkStateChanged,this,&MyCheckBox::onCheckStateChg);
}

void MyCheckBox::onCheckStateChg(Qt::CheckState newState)
{
    if (newState == Qt::Checked) {
        icon_Now = m_icon_Checked;
        emit sendDeleteRow(true);
    }
    else if (newState == Qt::Unchecked) {
        icon_Now = m_icon_Unchecked;
        emit sendDeleteRow(false);
    }
}

void MyCheckBox::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap;
    pixmap.load(icon_Now);
    painter.drawPixmap(rect(),pixmap);
}

bool MyCheckBox::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::Enter:
        if (checkState() == Qt::Unchecked) {
            icon_Now = m_icon_Hover;
        }
        break;
    case QEvent::Leave:
        if (checkState() == Qt::Unchecked) {
            icon_Now = m_icon_Unchecked;
        }
        break;
    default:
        break;
    }
    return QCheckBox::event(e);
}
