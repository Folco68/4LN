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

#ifndef WIDGETDESTINATION_HPP
#define WIDGETDESTINATION_HPP

#include <QCheckBox>
#include <QStorageInfo>
#include <QString>
#include <QWidget>

namespace Ui {
class Destination;
}

//
//  WidgetDestination
//
// This widget contains a checkbox and a drive letter. It allows the user to set the drives to be cloned
//
class WidgetDestination : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetDestination(QStorageInfo storage);
    ~WidgetDestination();

    bool isSelected() const;
    QCheckBox* checkBox() const;
    QString drivePath() const;

private:
    Ui::Destination* ui;
    QString RootPath;
};

#endif // WIDGETDESTINATION_HPP
