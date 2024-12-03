/*
 * 4LN - Multiple Destinations Copier - batch copier to clone a content into several drives at once
 * Copyright (C) 2020-2025 Martial Demolins AKA Folco
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

#include "WidgetDrop.hpp"
#include "ui_WidgetDrop.h"
#include <QFileInfo>
#include <QList>
#include <QMimeData>
#include <QUrl>

WidgetDrop::WidgetDrop(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::WidgetDrop)
{
    ui->setupUi(this);
}

WidgetDrop::~WidgetDrop()
{
    delete ui;
}

//
//  dragEnterEvent
//
// Tell the SourceBox to accept only one directory
//

void WidgetDrop::dragEnterEvent(QDragEnterEvent* event)
{
    // We want only URLs
    if (!event->mimeData()->hasUrls()) {
        return;
    }

    // We want only one URL
    QList<QUrl> List = event->mimeData()->urls();
    if (List.count() != 1) {
        return;
    }

    // We want only a directory
    QString Directory = List.at(0).toLocalFile();
    if (!QFileInfo(Directory).isDir()) {
        return;
    }

    event->acceptProposedAction();
}

//
//  dropEvent
//
// Display the selected directory and send a signal caught by the MainWindow
//

void WidgetDrop::dropEvent(QDropEvent* event)
{
    QString Directory = event->mimeData()->urls().at(0).toLocalFile();
    emit directoryDropped(Directory);
}
