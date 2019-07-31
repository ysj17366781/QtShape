#include "hexspinbox.h"
#include "ui_hexspinbox.h"

HexSpinBox::HexSpinBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HexSpinBox)
{
    ui->setupUi(this);
}

HexSpinBox::~HexSpinBox()
{
    delete ui;
}
