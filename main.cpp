#include "mainwindow.h"
#include <QApplication>

/*
 * 背景音乐（开始游戏后播放，暂停游戏停止）|ok
 * 失败提示|ok
 * 吃掉音效|ok
 * 设置界面（选择地图，|ok
 * 调节音乐音量，|ok
 * 换地图|ok
 * 奖励改一下外观|ok
 * 简单模式ok
 * 1，可以穿墙
 * 2，奖励不消失
 * 困难模式ok
 * 1，一定时间内奖励会重新改变位置刷新
 * 2，一定时间内蛇身会变短，当短到一定程度蛇饿死->也就是规定时间吃不到反而会变短
 * 3，不能穿墙但是可以接触自己的身体
 * 4，为了不让游戏太过于困难，一定时间内刷新大礼包，存在时间短，但是奖励丰厚
 * 修复bug（蛇初始化，重新开始游戏重新加载蛇）|ok
 * 最有一个bug：背景音乐播放不了，解析文件错误
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow;
    w->show();
    return a.exec();
}
