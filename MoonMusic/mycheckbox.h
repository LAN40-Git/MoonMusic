#ifndef MYCHECKBOX_H
#define MYCHECKBOX_H

#include <QCheckBox>
#include <QIcon>
#include <QPainter>
#include <QPaintEvent>

class MyCheckBox : public QCheckBox
{
    Q_OBJECT
    QString icon_Now = ":/icon/rect_grey.png";
    QString m_icon_Checked = ":/icon/checked.png";
    QString m_icon_Unchecked = ":/icon/rect_grey.png";
    QString m_icon_Hover = ":/icon/rect_hover.png";
    Qt::CheckState state = Qt::Unchecked;
public:
    ~MyCheckBox();
    MyCheckBox();
    QString getIcon_Now() const;
    void setIcon_Now(const QString &newIcon_Now);
private slots:
    void onCheckStateChg(Qt::CheckState newState);

signals:
    void sendDeleteRow(bool status);
protected:
    void paintEvent(QPaintEvent *event) override;

    bool event(QEvent *e) override;
};

#endif // MYCHECKBOX_H
