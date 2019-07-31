#ifndef HEXSPINBOX_H
#define HEXSPINBOX_H

#include <QWidget>
#include <QDesignerCustomWidgetInterface>


namespace Ui {
class HexSpinBox;
}

class HexSpinBox : public QWidget
{
    Q_OBJECT

public:
    explicit HexSpinBox(QWidget *parent = nullptr);
    ~HexSpinBox();

private:
    Ui::HexSpinBox *ui;
};

#endif // HEXSPINBOX_H
