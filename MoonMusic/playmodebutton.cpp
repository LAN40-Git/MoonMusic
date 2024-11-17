#include "playmodebutton.h"

PlayModeButton::PlayModeButton()
{
    setFixedSize(30,30);
    setIconSize(QSize(17,17));
    setCursor(Qt::PointingHandCursor);
    setStyleSheet("border:none;background:transparent;border-radius:0;");
    connect(this,&PlayModeButton::clicked,this,&PlayModeButton::onBtnClicked);
}

void PlayModeButton::init_PlayMode(PlayMode newMode)
{
    playMode = newMode;//初始化播放模式
    emit sendPlayMode(playMode);
    set_Icon(playMode);
    setIcon(QIcon(icon_common));
}

void PlayModeButton::set_Icon(PlayMode newMode)
{
    switch (newMode) {//根据不同播放模式设置图标
    case PlayMode::SequenceMode://顺序播放
        icon_common = icon_Sequence_common;
        icon_hover = icon_Sequence_hover;
        icon_pressed = icon_Sequence_pressed;
        break;
    case PlayMode::SingleLoopMode://随机播放
        icon_common = icon_SingleLoop_common;
        icon_hover = icon_SingleLoop_hover;
        icon_pressed = icon_SingleLoop_pressed;
        break;
    case PlayMode::RandomMode://单曲循环
        icon_common = icon_Random_common;
        icon_hover = icon_Random_hover;
        icon_pressed = icon_Random_pressed;
        break;
    default:
        break;
    }
}

void PlayModeButton::onBtnClicked()
{
    //顺序播放->单曲循环->随机播放->顺序播放...
    switch (playMode) {
    case PlayMode::SequenceMode:
        playMode = PlayMode::SingleLoopMode;
        emit sendPlayMode(playMode);
        set_Icon(playMode);
        break;
    case PlayMode::SingleLoopMode:
        playMode = PlayMode::RandomMode;
        emit sendPlayMode(playMode);
        set_Icon(playMode);
        break;
    case PlayMode::RandomMode:
        playMode = PlayMode::SequenceMode;
        emit sendPlayMode(playMode);
        set_Icon(playMode);
        break;
    default:
        break;
    }
    setIcon(QIcon(icon_hover));
}

bool PlayModeButton::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::Enter:
        setIcon(QIcon(icon_hover));
        break;
    case QEvent::Leave:
        setIcon(QIcon(icon_common));
        break;
    case QEvent::MouseButtonPress:
        setIcon(QIcon(icon_pressed));
        break;
    default:
        break;
    }

    return QPushButton::event(e);
}
