#include "Destination.hpp"
#include "ui_Destination.h"

Destination::Destination(QStorageInfo storage) : ui(new Ui::Destination)
{
    ui->setupUi(this);
    ui->Checkbox->setText(storage.name());
}

Destination::~Destination()
{
    delete ui;
}
