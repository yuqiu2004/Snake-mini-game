#ifndef HARDMODE_H
#define HARDMODE_H

#include <QWidget>
#include "direction.h"
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QSound>
#include <QDebug>
#include <QString>
#include <QMediaPlayer>
#include <QUrl>

namespace Ui {
class HardMode;
}
//困难模式的窗口
class HardMode : public QWidget
{
    Q_OBJECT
    friend class MainWindow;

public:
    explicit HardMode(int map, QWidget *parent = nullptr);
    ~HardMode();

protected://成员函数
    void paintEvent(QPaintEvent *event);//渲染，绘图
    void addTop();
    void addDown();
    void addLeft();
    void addRight();
    void deleteLast();
    void updateSnake();//初始化蛇
    void addReward();//添加奖品
    void addBonus();
    void keyPressEvent(QKeyEvent *event);//按键处理
    bool isDead();//判断是否碰触蛇身

private:
    Ui::HardMode *ui;
    int moveFlag;
    bool gameStart = false;
    bool isDefeated;//记录是否已经结束
    int allowed_Width;//边界
    int allowed_Height;
    int nodeWidth = 20;//小方块的大小
    int nodeHeight = 20;
    int rewardWidth = 20;//奖品大小
    int rewardHeight = 20;
    int score;//记录分数

    QList<QRectF> snake; //蛇容器-QRectF表示一块区域
    QRect *rect;//起始的位置
    QTimer *timer;//定时器
    int time = 100;//超时时间是100ms
    int timeRe = 6000;//5s奖励更新
    int recordRe = 0;
    int timeBo = 9000;//7秒刷一次大奖
    int recordBo = 0;
    int timeMaintain = 3000;//大奖保持时间
    int recordMa = 0;
    int timeLo = 7000;//5s蛇身变短一次
    int recordLo = 0;
    QRectF rewardNode;//奖品
    QRectF bonusNode;
    bool isBonusExit = false;
    QSound *eat;//音效
    QString map;//地图
    QString col;

signals:
    void backTo(void);
    void music_pause(void);
    void music_begin(void);

protected slots:
    void timeout();
};

#endif // HARDMODE_H
