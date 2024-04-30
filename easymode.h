#ifndef EASYMODE_H
#define EASYMODE_H

#include <QWidget>
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
#include "direction.h"

namespace Ui {
class EasyMode;
}

class EasyMode : public QWidget
{
    Q_OBJECT
    friend class MainWindow;

public:
    explicit EasyMode(int mapInt, QWidget *parent = nullptr);
    ~EasyMode();

private:
    Ui::EasyMode *ui;

protected:
    void addTop();
    void addDown();
    void addLeft();
    void addRight();
    void deleteLast();
    void updateSnake();
    void addReward();
    void addBonus();
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    bool isContact();

private:
    int moveFlag;
    bool gameStart = false;
    bool isDefeated;
    int allowed_Width;
    int allowed_Height;
    int nodeWidth = 20;
    int nodeHeight = 20;
    int rewardWidth = 20;
    int rewardHeight = 20;
    int score;
    QList<QRectF> snake;
    QRect *rect;
    QTimer *timer;
    int time = 100;
    QRectF rewardNode;
    QSound *eat;
    QString map;
    QString col;

signals:
    void backTo(void);
    void music_pause(void);
    void music_begin(void);

protected slots:
    void timeout();
};

#endif // EASYMODE_H
