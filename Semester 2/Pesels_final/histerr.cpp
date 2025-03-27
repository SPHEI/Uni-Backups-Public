#include "histerr.h"
#include "ui_histerr.h"

HistErr::HistErr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistErr)
{
    ui->setupUi(this);
}

HistErr::~HistErr()
{
    delete ui;
}
