#include "selectplaylistbutton.h"

#include <QFontDatabase>
#include <QStringLiteral>

SelectPlayListButton::SelectPlayListButton(const QString &playListName, const QString &path_Common, const QString &path_Hover)
{
    //初始化按钮图标
    hover = path_Hover;
    common = path_Common;
    btnIcon.load(path_Common);

    //初始化按钮字体
    fontId = QFontDatabase::addApplicationFont(QStringLiteral(":/font/Alibaba_PuHuiTi_2.0_55_Regular_55_Regular.ttf"));
    fontFamily = QFontDatabase::applicationFontFamilies(fontId);
    font = QFont(fontFamily[0],10,QFont::Medium);

    //初始化按钮以及文本绘制区域
    textRect = QRectF(rect().x()+44,rect().y()+8,rect().width()-50,rect().height());
    iconRect = QRect(rect().x()+16,rect().y()+8,17,17);
    if (common == ":/icon/allmusic.png") {
        iconRect = QRect(rect().x()+15,rect().y()+8,20,20);
    }
    if (common == ":/icon/heart.png") {
        iconRect = QRect(rect().x()+15,rect().y()+8,16,15);
    }
    //初始化按钮文本
    name = playListName;
    if (playListName.length() >= MaxLength) {
        name_exc = playListName.left(MaxLength)+"...";
    } else {
        name_exc = playListName;
    }

    //初始化按钮属性
    setCheckable(true);
    setFixedSize(140,32);
    setCursor(Qt::PointingHandCursor);

    connect(&timer_color_enter,&QTimer::timeout,this,&SelectPlayListButton::onTimer_Color_Enter);
    connect(&timer_color_leave,&QTimer::timeout,this,&SelectPlayListButton::onTimer_Color_Leave);
    connect(&timer_radius_enter,&QTimer::timeout,this,&SelectPlayListButton::onTimer_Radius_Enter);
    connect(&timer_radius_leave,&QTimer::timeout,this,&SelectPlayListButton::onTimer_Radius_Leave);
    connect(this,&QPushButton::clicked,[&](){
        emit playListChanged(name);//点击当前按钮时，将当前歌单名发出
    });
    connect(this,&SelectPlayListButton::toggled,this,onSelectPlayListButtonToggled);
}

void SelectPlayListButton::animation_Enter()
{
    btnIcon.load(hover);
    if (timer_color_leave.isActive()) {
        timer_color_leave.stop();
    }
    if (timer_radius_leave.isActive()) {
        timer_radius_leave.stop();
    }
    timer_color_enter.start(2);
    timer_radius_enter.start(20);
}

void SelectPlayListButton::animation_Leave()
{
    btnIcon.load(common);
    if (timer_color_enter.isActive()) {
        timer_color_enter.stop();
    }
    if (timer_radius_enter.isActive()) {
        timer_radius_enter.stop();
    }
    timer_color_leave.start(3);
    timer_radius_leave.start(20);
}

void SelectPlayListButton::onTimer_Color_Enter()
{
    btnColor--;
    fontColor++;
    if (btnColor <= 34 || fontColor >=255) {
        timer_color_enter.stop();
    }
    update();
}

void SelectPlayListButton::onTimer_Color_Leave()
{
    btnColor++;
    fontColor--;
    if (fontColor <= 34 || btnColor >=255) {
        timer_color_leave.stop();
    }
    update();
}

void SelectPlayListButton::onTimer_Radius_Enter()
{
    cornerRadius++;
    if (cornerRadius >= 10) {
        timer_radius_enter.stop();
    }
    update();
}

void SelectPlayListButton::onTimer_Radius_Leave()
{
    cornerRadius--;
    if (cornerRadius <= 2) {
        timer_radius_leave.stop();
    }
    update();
}

void SelectPlayListButton::onSelectPlayListButtonToggled(bool checked)
{
    if (checked) {
        selected = true;
    } else {
        animation_Leave();
        selected = false;
    }
}

void SelectPlayListButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);//文字抗锯齿

    //绘制按钮矩形
    QPainterPath path;
    firstRect = QRectF(rect().x(),rect().y(),cornerRadius*2,cornerRadius*2);
    secondRect = QRectF(rect().right()-(cornerRadius*2),rect().bottom()-(cornerRadius*2),cornerRadius*2,cornerRadius*2);
    path.moveTo(firstRect.x()+cornerRadius,firstRect.y());
    path.arcTo(firstRect,90,90);
    path.lineTo(rect().x(),rect().bottom());
    path.lineTo(secondRect.x()+cornerRadius,secondRect.bottom());
    path.arcTo(secondRect,270,90);
    path.lineTo(rect().right(),rect().y());
    path.lineTo(firstRect.x()+cornerRadius,firstRect.y());

    painter.setPen(QColor(btnColor,btnColor,btnColor));
    painter.setBrush(QColor(btnColor,btnColor,btnColor));
    painter.drawPath(path);

    //绘制字体
    painter.setPen(QColor(fontColor,fontColor,fontColor));
    painter.setFont(font);
    painter.drawText(textRect,name_exc);
    // qreal pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真
    // btnIcon = btnIcon.scaled(QSize(width() * pixelRatio, height() *pixelRatio)
    //                              , Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//缩放图片到当前分辨率下的显示大小，SmoothTransformation平滑处理
    painter.drawPixmap(iconRect,btnIcon);
}

bool SelectPlayListButton::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::Enter:
        if (!selected) {
            animation_Enter();
        }
        break;
    case QEvent::Leave:
        if (!selected) {
            animation_Leave();
        }
        break;
    default:
        break;
    }
    return QPushButton::event(e);
}
