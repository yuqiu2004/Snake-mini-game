#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QPainter>
#include <QMessageBox>
#include <QCloseEvent>
#include "easymode.h"
#include "hardmode.h"
#include "setwidget.h"
#include <QPalette>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;//ui成员

    EasyMode *gameWidget;//简单模式
    HardMode *gameWidget2;//困难模式
    SetWidget *setWidget;//设置窗口
    QMediaPlayer *music;
    QUrl *url;

    bool gameStart;
    int map;//代表地图代号
    int volume;

protected:
    void paintEvent(QPaintEvent *ev);
    void closeEvent(QCloseEvent *ev);
public slots:
    void handleError(QMediaPlayer::Error error);
};

#endif // MAINWINDOW_H
