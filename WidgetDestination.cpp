#include "WidgetDestination.hpp"
#include "ui_WidgetDestination.h"
#include <QDir>

WidgetDestination::WidgetDestination(QStorageInfo storage) : ui(new Ui::Destination)
{
    ui->setupUi(this);

    // Root path return the drive letter
    this->RootPath = QDir::cleanPath(QDir(storage.rootPath()).canonicalPath());

    // Add the volume label if one is set
    if (storage.name().isEmpty()) {
        ui->Checkbox->setText(this->RootPath);
    }
    else {
        ui->Checkbox->setText(QString("%1 (%2)").arg(this->RootPath).arg(storage.name()));
    }
}

WidgetDestination::~WidgetDestination()
{
    delete ui;
}

QCheckBox* WidgetDestination::checkBox() const
{
    return ui->Checkbox;
}

bool WidgetDestination::isSelected() const
{
    return ui->Checkbox->isChecked();
}

QString WidgetDestination::drivePath() const
{
    return this->RootPath;
}
