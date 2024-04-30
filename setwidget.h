#ifndef SETWIDGET_H
#define SETWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QPalette>
#include <QString>
#include <QDebug>
#include <QButtonGroup>

namespace Ui {
class SetWidget;
}

class SetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SetWidget(QWidget *parent = nullptr);
    ~SetWidget();
    int getMap();
    int getVolume();

private:
    Ui::SetWidget *ui;
    QPixmap *bg;
    QPainter *painter;
    QButtonGroup *group;

protected:
    void paintEvent(QPaintEvent *ev);

signals:
    void backTo();
    void set();
};

#endif // SETWIDGET_H
