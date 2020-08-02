#include "Destination.hpp"
#include "ui_Destination.h"

Destination::Destination(QStorageInfo storage) : ui(new Ui::Destination)
{
    ui->setupUi(this);

    // Root path return the drive letter
    this->RootPath = storage.rootPath();

    // Add the volume label if one is set
    if (storage.name().isEmpty()) {
        ui->Checkbox->setText(this->RootPath);
    }
    else {
        ui->Checkbox->setText(QString("%1 (%2)").arg(this->RootPath).arg(storage.name()));
    }
}

Destination::~Destination()
{
    delete ui;
}

QCheckBox* Destination::checkBox() const
{
    return ui->Checkbox;
}

bool Destination::isSelected() const
{
    return ui->Checkbox->isChecked();
}

QString Destination::drivePath() const
{
    return this->RootPath;
}
