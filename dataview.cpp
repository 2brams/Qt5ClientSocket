#include "dataview.h"
#include "ui_dataview.h"

Dataview::Dataview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dataview)
{
    ui->setupUi(this);
}

Dataview::~Dataview()
{
    delete ui;
}
