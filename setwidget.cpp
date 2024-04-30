#include "setwidget.h"
#include "ui_setwidget.h"

/*
 * 实现设置背景图和调节音量的功能
 * 1，调节音量
 * 2，设置背景图
 * 如何刷新：
 * 通过信号连接，当按下应用按钮的时候释放一个信号，只有游戏窗口检测到该信号，就重新设置音量和背景图
 * 所以连接在游戏窗口中实现（构造函数）
*/

SetWidget::SetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetWidget)
{
    ui->setupUi(this);
    //固定大小
    this->setFixedSize(1000,600);
    //窗口标题
    this->setWindowTitle("贪吃蛇");
    //窗口图标
    this->setWindowIcon(QIcon(":/img_widgetIcon/widget_icon.webp"));

    //设置音量范围
    ui->horizontalSlider->setRange(0,100);
    //设置最小变化数值
    ui->horizontalSlider->setSingleStep(5);
    //设置当前值
    ui->horizontalSlider->setValue(50);
    //

    connect(ui->pushButton,&QPushButton::clicked,this,&SetWidget::backTo);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&SetWidget::set);//按下应用才设置成功

    group = new QButtonGroup(this);
    group->addButton(ui->radioButton);
    group->addButton(ui->radioButton_2);
    group->addButton(ui->radioButton_3);
}

SetWidget::~SetWidget()
{
    delete bg;
    delete painter;
    delete ui;
}

void SetWidget::paintEvent(QPaintEvent *event){
        //窗口背景
    bg = new QPixmap(":/img_background/bg_green.jpg");
    painter = new QPainter(this);
    painter->drawPixmap(0,0,1000,600,*bg);
    painter->end();
}

int SetWidget::getMap(){
    QString map;
    if(group->checkedButton()==nullptr){
        return 0;
    }
    map = group->checkedButton()->text();//获取选中的内容

    if(map == "Reality"){
        return 0;
    }else if(map == "BlackWorld"){
        return 1;
    }else if(map == "BlueWorld"){
        return 2;
    }else{
        return 0;
    }

}

int SetWidget::getVolume(){
    return ui->horizontalSlider->value();
}
