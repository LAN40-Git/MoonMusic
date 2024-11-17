#ifndef SELECTPLAYLISTBUTTON_H
#define SELECTPLAYLISTBUTTON_H
#define MaxLength 6 //按钮内字体最大长度

#include <QPushButton>
#include <QPainter>
#include <QPaintEvent>
#include <QEvent>
#include <QRectF>
#include <QBrush>
#include <QFont>
#include <QPainterPath>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <QFont>

class SelectPlayListButton : public QPushButton
{
    Q_OBJECT
    int fontId;
    QStringList fontFamily;
    QFont font;
    bool selected = false;
    QRectF textRect;//文本绘制区域
    QRect iconRect;//图标绘制区域
    QString name;//按钮文本
    QString name_exc;//省略后的文本
    QPixmap btnIcon;//按钮图标
    QString common;//正常状态下的图标的url
    QString hover;//悬浮状态下的图标的url
    QRectF firstRect;//按钮左上角矩形，用于绘制圆角
    QRectF secondRect;//按钮右下角矩形，用于绘制圆角
    int fontColor=34;//字体初始rgb值
    int btnColor=255;//按钮初始rgb值
    int cornerRadius = 2;//按钮初始圆角
    QTimer timer_color_enter;
    QTimer timer_color_leave;
    QTimer timer_radius_enter;
    QTimer timer_radius_leave;
signals:
    void playListChanged(const QString &playListName);
public:
    SelectPlayListButton(const QString &playListName, const QString &path, const QString &path_Hover);
    void animation_Enter();
    void animation_Leave();
public slots:
    void onTimer_Color_Enter();
    void onTimer_Color_Leave();
    void onTimer_Radius_Enter();
    void onTimer_Radius_Leave();
    void onSelectPlayListButtonToggled(bool checked);
protected:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *e) override;
};

#endif // SELECTPLAYLISTBUTTON_H
