#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);//初始化ui

    this->setFixedSize(1000,600);//设置窗口大小
    this->setWindowIcon(QIcon(":/img_widgetIcon/widget_icon.webp"));//设置窗口图标
    this->setWindowTitle("贪吃蛇");//设置窗口标题

    QPalette pe;//调色板
    pe.setColor(QPalette::WindowText,Qt::white);//设置颜色
    ui->label->setPalette(pe);

    //初始化游戏状态
    gameStart = false;

    //初始化地图和音量
    map = 0;
    volume = 50;
    music = new QMediaPlayer(this);
    url = new QUrl("qrc:/music/bg_music.wav");//url复制路径不要用path
//    music->setMedia(QUrl::fromLocalFile("D:\\Study\\QT\\projects\\snake_Game\\bg_music.wav"));
    music->setMedia(*url);
    music->setVolume(volume);
    //设置音乐死循环
    connect(music,&QMediaPlayer::stateChanged,this,[&](QMediaPlayer::State state){
        if(gameStart && QMediaPlayer::StoppedState == state){
            music->play();//如果停止就重新播放，但是这需要在游戏开始状态下
        }
    });

    //捕捉错误信息
    connect(music,SIGNAL(error(QMediaPlayer::Error)),this,SLOT(handleError(QMediaPlayer::Error)));

    //打开设置界面
    connect(this->ui->pushButton,&QPushButton::clicked,this,[&]{
        setWidget = new SetWidget;
        connect(setWidget,&SetWidget::set,this,[&]{
            map = setWidget->getMap();
            volume = setWidget->getVolume();
            this->music->setVolume(volume);
        });
        connect(setWidget,&SetWidget::backTo,this,[&]{
            setWidget->hide();
            this->show();
            delete setWidget;
        });
        this->hide();
        setWidget->show();
    });

    //开始简单模式游戏，切换到游戏窗口
    connect(ui->pushButton_1,&QPushButton::clicked,this,[&](){
        gameWidget = new EasyMode(map);
        gameStart = true;
        connect(gameWidget,&EasyMode::backTo,this,[&]{
            gameStart = false;
            this->show();
            gameWidget->hide();
            this->music->stop();
            delete gameWidget;
        });//返回信号
        //处理音乐
        connect(gameWidget,&EasyMode::music_pause,this,[&]{
            gameStart = false;
            this->music->stop();
        });
        connect(gameWidget,&EasyMode::music_begin,this,[&]{
            gameStart = true;
            this->music->play();
        });
        this->hide();
        gameWidget->show();
    });
    //开始困难模式游戏，切换到游戏窗口
    connect(ui->pushButton_2,&QPushButton::clicked,this,[&](){
        gameWidget2 = new HardMode(map);
        gameStart = true;
        connect(gameWidget2,&HardMode::backTo,this,[&]{
            gameStart = false;
            this->show();
            gameWidget2->hide();
            this->music->stop();
            delete gameWidget2;
        });//返回信号
        //处理音乐
        connect(gameWidget2,&HardMode::music_pause,this,[&]{
            gameStart = false;
            this->music->stop();
        });
        connect(gameWidget2,&HardMode::music_begin,this,[&]{
            gameStart = true;
            this->music->play();
        });
        this->hide();
        gameWidget2->show();
    });

    //退出游戏///////////////////////////////////////////////////
    connect(ui->pushButton_3,&QPushButton::clicked,this,&QWidget::close);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete music;
    delete url;
}

//绘画事件
void MainWindow::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    QPixmap pix;

    pix.load(":/img_background/bg_main.jpg");
    painter.drawPixmap(0, 0, 1000, 600, pix);
}

//关闭窗口提示
void MainWindow::closeEvent(QCloseEvent *ev)
{
    int ret = QMessageBox::question(this,"提示","您确定要关闭窗口吗");
    if(ret == QMessageBox::Yes){
        ev->accept();
    }
    else{
        ev->ignore();
    }
}

void MainWindow::handleError(QMediaPlayer::Error error)
{
    qDebug()<<"Error occured:"<<error<<endl;
}

