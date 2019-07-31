#include "gradient.h"
#include "ui_form.h"
#include <QKeyEvent>


MyWidget::MyWidget(QWidget *parent):QWidget (parent), ui(new Ui::Form)
{
    ui->setupUi(this);

    ui->pushButton->installEventFilter(this);
    ui->pushButton_2->installEventFilter(this);
}

bool MyWidget::eventFilter(QObject *obj, QEvent *evt)
{
    if (obj == ui->pushButton || obj == ui->pushButton_2)
    {
        if (evt->type() == QEvent::KeyPress)
        {
            QKeyEvent *kv = static_cast<QKeyEvent *>(evt);
            if (kv->key() == Qt::Key_Space)
            {
                focusNextChild();
                return true;
            }
        }
    }

    return QWidget::eventFilter(obj, evt);
}

void MyWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QLinearGradient gradient(50, 100, 300,350);
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(0.2, Qt::green);
    gradient.setColorAt(0.6, Qt::red);
    gradient.setColorAt(1.0, Qt::yellow);
    painter.setBrush(QBrush(gradient));
    painter.drawEllipse(10,10,200,200);

    QPoint pos(100,50);
    QTransform transform;
    transform.translate(50,50);
    transform.rotate(45.0);
    painter.setWorldTransform(transform);
    painter.setPen(Qt::red);
    painter.drawText(pos, tr("Circle"));
}
