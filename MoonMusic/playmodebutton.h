#ifndef PLAYMODEBUTTON_H
#define PLAYMODEBUTTON_H

#include <QPushButton>
#include <QEvent>

/*播放模式按钮类
 * 传入一个初始播放模式
 * 后继通过接收按钮点击事件自动切换播放模式
 * 相应的按钮图标和大小也会改变
*/

enum class PlayMode
{
    SequenceMode=0,//顺序播放
    SingleLoopMode,//单曲循环
    RandomMode//随机播放
};

class PlayModeButton : public QPushButton
{
    Q_OBJECT
    PlayMode playMode;

    QString icon_common;
    QString icon_hover;
    QString icon_pressed;

    // 顺序播放按钮图标
    QString icon_Sequence_common=":/icon/sequence.png";
    QString icon_Sequence_hover=":/icon/sequence2.png";
    QString icon_Sequence_pressed=":/icon/sequence3.png";

    // 单曲循环按钮图标
    QString icon_SingleLoop_common=":/icon/singleloop.png";
    QString icon_SingleLoop_hover=":/icon/singleloop2.png";
    QString icon_SingleLoop_pressed=":/icon/singleloop3.png";

    // 随机播放按钮图标
    QString icon_Random_common=":/icon/random.png";
    QString icon_Random_hover=":/icon/random2.png";
    QString icon_Random_pressed=":/icon/random3.png";

public:
    PlayModeButton();

    void init_PlayMode(PlayMode newMode);

    void set_Icon(PlayMode newMode);

    void onBtnClicked();

signals:
    void sendPlayMode(PlayMode mode);

protected:
    bool event(QEvent *e) override;
};

#endif // PLAYMODEBUTTON_H
