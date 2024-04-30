#include "hardmode.h"
#include "ui_hardmode.h"

HardMode::HardMode(int mapInt, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HardMode)
{
    //qDebug()<<"构造开始执行"<<endl;
    switch(mapInt){
        case 0:
            map = ":/img_background/bg_0.webp";
            col = ":/img_background/scoreBoard1.png";
            break;
        case 1:
            map = ":/img_background/bg_black.jpg";
            col = ":/img_background/scoreBoard2.png";
            break;
        case 2:
            map = ":/img_background/bg_blue.jpg";
            col = ":/img_background/scoreBoard3.png";
            break;
        default:
            map = ":/img_background/bg_0.webp";
            col = ":/img_background/scoreBoard1.png";
            break;
    }
    //初始化设置窗口
    moveFlag = DIR_RIGHT;
    this->setWindowIcon(QIcon(":/img_widgetIcon/widget_icon.webp"));//设置游戏窗口的图标
    ui->setupUi(this);//设置ui的父对象、设置窗口固定大小、设置窗口标题
    this->setFixedSize(1000,600);
    this->setWindowTitle("贪吃蛇");

    ui->label_2->setStyleSheet("color:#ffffff;");

    //创建定时器
    timer = new QTimer;
    timer->stop();

    //定时器有一个timeout信号，信号连接信号
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));

    //初始化游戏状态，初始化为击败状态（上一把的末状态）
    isDefeated = true;

    //初始化蛇身位置
    rect = new QRect(300,180,nodeWidth,nodeHeight);


    //设置蛇的活动边界
    allowed_Width = 900;
    allowed_Height = 600;

    //初始化音效
    eat = new QSound(":/music/eat_voice.wav");
    eat->setLoops(1);
    eat->play();

    //初始化奖品,添加一个奖品
    addReward();

    //初始化分数
    score = 0;

    //信号连接信号（返回信号）
    connect(this->ui->pushButton,SIGNAL(clicked(bool)),this,SIGNAL(backTo()));

    //取消按钮聚焦（防止空格键被按钮劫持）
    ui->pushButton->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_2->setFocusPolicy(Qt::NoFocus);

    //信号连接-重新开始游戏
    connect(ui->pushButton_2,&QPushButton::clicked,this,[&](){
        //qDebug()<<"按下重新开始"<<endl;
        isDefeated = false;//失败状态
        gameStart = false;//游戏开始状态
        score = 0;//分数刷新
        updateSnake();
        ui->label_2->clear();
    });
    //qDebug()<<"构造结束"<<endl;
}

HardMode::~HardMode()
{
    //qDebug()<<"析构执行"<<endl;
    //回收所有堆变量
    delete ui;
    delete rect;
    delete timer;
    delete eat;

}

void HardMode::updateSnake(){
    //qDebug()<<"刷新蛇"<<endl;
    if(!this->snake.empty())//不为空
        this->snake.clear();//清空链表
    moveFlag = DIR_RIGHT;
    snake.append(*rect);
    addRight();
    addRight();
    addRight();
    //qDebug()<<"刷新结束"<<endl;
}

void HardMode::keyPressEvent(QKeyEvent *event)
{
    //qDebug()<<"KeyPressEvent"<<endl;
    if(!isDefeated){
        switch(event->key()){
            case Qt::Key_Up:
                if(moveFlag != DIR_DOWN)
                    moveFlag = DIR_UP;
                break;
            case Qt::Key_Down:
                if(moveFlag != DIR_UP)
                    moveFlag = DIR_DOWN;
                break;
            case Qt::Key_Left:
                if(moveFlag != DIR_RIGHT)
                    moveFlag = DIR_LEFT;
                break;
            case Qt::Key_Right:
                if(moveFlag != DIR_LEFT)
                    moveFlag = DIR_RIGHT;
                break;
            case Qt::Key_Space:
                if(gameStart){
                    //qDebug()<<"暂停"<<endl;
                    gameStart = false;
                    timer->stop();
                    emit music_pause();
                }
                else{
                    //qDebug()<<"继续"<<endl;
                    gameStart = true;
                    //启动定时器
                    timer->start(time);
                    emit music_begin();
                }
                    break;
            default:
                break;
        }
    }
}

bool HardMode::isDead()
{
    //qDebug()<<"bool isContact"<<endl;
    if(snake.size() == 1){
        return true;
    }else{
        switch (moveFlag) {
            case DIR_UP:
                if(snake[0].y()-nodeHeight < 0){
                    return true;
                }
                break;
            case DIR_DOWN:
                if((snake[0].y()+nodeHeight*2) > allowed_Height){
                    return true;
                }
                break;
            case DIR_LEFT:
                if((snake[0].x() - nodeWidth) < 0){
                    return true;
                }
                break;
            case DIR_RIGHT:
                if((snake[0].x() + nodeWidth) > allowed_Width - nodeWidth){
                    return true;
                }
                break;
        }
    }
    return false;
}

void HardMode::timeout()
{
    //qDebug()<<"timeout"<<endl;
    //先判断是否吃到自己
    if(isDead()){//自己和自己重合，应该更新状态不然会持续处于重合状态
        isDefeated = true;
        gameStart = false;
        timer->stop();//计时器停止
        ui->label_2->setText("游戏失败！");
        emit music_pause();
    }
    //时间计算
    if(recordRe == timeRe){
        addReward();
        recordRe = 0;
    }else{
        recordRe += time;
    }
    if(recordLo == timeLo){
        deleteLast();
        recordLo = 0;
    }else{
        recordLo += time;
    }
    if(isBonusExit){
        if(recordMa == timeMaintain){
            recordMa = 0;
            isBonusExit = false;
        }else{
            recordMa += time;
            recordBo += time;
        }
    }else{
        if(recordBo == timeBo){
            isBonusExit = true;
            addBonus();
            recordBo = 0;
        }else{
            recordBo += time;
        }
    }
    int num = 1;
    //判断有没有重合
    if(snake[0].intersects(rewardNode)){
        //播放音效
        //qDebug()<<"吃到奖励"<<endl;
        eat->play();
        num = 2;
        addReward();
        recordRe = 0;
        score++;
    }
    if(isBonusExit && snake[0].intersects(bonusNode)){
        //播放音效
        //qDebug()<<"吃到bonus奖励"<<endl;
        eat->play();
        num = 4;
        isBonusExit = false;
        score += 3;
    }
    while(num--){
        switch(moveFlag){
            case DIR_UP:
                //在顶部加一个方块
                addTop();
                break;
            case DIR_DOWN:
                addDown();
                break;
            case DIR_LEFT:
                addLeft();
                break;
            case DIR_RIGHT:
                addRight();
                break;
        }
    }
    //删除最后一个
    deleteLast();
    update();
}

void HardMode::addTop()
{
    //qDebug()<<"addTop"<<endl;
    QPointF leftTop;
    QPointF rightBottom;
    leftTop = QPointF(snake[0].x(), snake[0].y()-nodeHeight);
    rightBottom = snake[0].topRight();
    snake.insert(0,QRectF(leftTop, rightBottom));
}

void HardMode::addDown()
{
    //qDebug()<<"addDown"<<endl;
    QPointF leftTop;
    QPointF rightBottom;
    leftTop = snake[0].bottomLeft();
    rightBottom = snake[0].bottomRight() + QPointF(0,nodeHeight);
    snake.insert(0,QRectF(leftTop, rightBottom));
}

void HardMode::addLeft()
{
     //qDebug()<<"addLeft"<<endl;
    QPointF leftTop;
    QPointF rightBottom;
    leftTop = snake[0].topLeft() - QPointF(nodeWidth,0);
    rightBottom = leftTop + QPointF(nodeWidth,nodeHeight);
    snake.insert(0,QRectF(leftTop, rightBottom));
}

void HardMode::addRight()
{
     //qDebug()<<"addRight"<<endl;
    QPointF leftTop;
    QPointF rightBottom;
    leftTop = snake[0].topLeft() + QPointF(nodeWidth,0);
    rightBottom = leftTop + QPointF(nodeWidth,nodeHeight);
    snake.insert(0,QRectF(leftTop, rightBottom));
}

void HardMode::deleteLast()
{
     //qDebug()<<"deleteLast"<<endl;
    snake.removeLast();
}

void HardMode::addReward()
{    //qDebug()<<"addReward"<<endl;
    rewardNode = QRectF(
                qrand()% (allowed_Width/20) * 20,
                qrand()%(allowed_Height/20) * 20,
                nodeWidth,
                nodeHeight);
}

void HardMode::addBonus(){
    bonusNode = QRectF(
                qrand()% (allowed_Width/20) * 20,
                qrand()%(allowed_Height/20) * 20,
                nodeWidth*2,
                nodeHeight*2);
}

void HardMode::paintEvent(QPaintEvent *event)
{
    //qDebug()<<"paintEvent"<<endl;
    QPainter painter(this);
    QPen pen;
    QBrush brush;

    //背景图片
    QPixmap pix;
    pix.load(map);
    painter.drawPixmap(0, 0, 900, 600, pix);

    //分数旁栏
    pen.setColor(Qt::black);
    QPixmap pix2;
    pix2.load(col);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawPixmap(900,0,100,600,pix2);

    //分数
    ui->lcdNumber->display(score);


    //画黑边
    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(0,0,5,600);
    painter.drawRect(900,0,5,600);
    painter.drawRect(1000,0,5,600);
    painter.drawRect(0,0,1000,5);
    painter.drawRect(0,600,1000,5);

    //画蛇
    if(!snake.empty()){
        pen.setColor(0XCC9966);
        brush.setColor(0Xffa631);
        brush.setStyle(Qt::SolidPattern);
        painter.setPen(pen);
        painter.setBrush(brush);
        //画头
        painter.drawEllipse(snake[0]);
        //画身
        brush.setColor(0XFFFF00);
        painter.setBrush(brush);
        for(int i = 1; i < snake.length(); i++){
            painter.drawEllipse(snake[i]);
        }
    }

    pen.setColor(Qt::red);
    painter.setPen(pen);
    brush.setColor(Qt::red);
    painter.setBrush(brush);

    //画奖品
    QPixmap pixOfReward;
    pixOfReward.load(":/reward/reward.png");
    painter.drawPixmap(rewardNode.x(),rewardNode.y(),rewardWidth,rewardHeight,pixOfReward);
    //画bonus
    if(isBonusExit){
        pixOfReward.load(":/reward/bonus.png");
        painter.drawPixmap(bonusNode.x(),bonusNode.y(),rewardWidth*2,rewardHeight*2,pixOfReward);
    }
    painter.end();
     //qDebug()<<"end of paint"<<endl;
}
