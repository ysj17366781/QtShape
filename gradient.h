#ifndef GRADIENT_H
#define GRADIENT_H
#include <QWidget>
#include <QLineEdit>
#include <QPainter>

namespace Ui {
class Form;
}

class MyWidget:public QWidget{
    Q_OBJECT
public:
    MyWidget(QWidget *parent=0);
    ~MyWidget(){};

    bool eventFilter(QObject *obj, QEvent *evt);
    void paintEvent(QPaintEvent *event);

private:
    Ui::Form *ui;
    QLineEdit *pedit1;
    QLineEdit *pedit2;

};

#endif // GRADIENT_H
