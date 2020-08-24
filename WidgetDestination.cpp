/*
 * MDC - Multiple Destinations Copier - batch copier to clone a content into several drives at once
 * Copyright (C) 2020 Martial Demolins AKA Folco
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * mail: martial <dot> demolins <at> gmail <dot> com
 */

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
