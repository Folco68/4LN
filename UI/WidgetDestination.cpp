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

#include "WidgetDestination.hpp"
#include "ui_WidgetDestination.h"
#include <QLayoutItem>
#include <QStorageInfo>

WidgetDestination::WidgetDestination(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::WidgetDestination)
{
    ui->setupUi(this);

    this->DestIcon = QIcon(":/Pictures/USBstick.png");
}

WidgetDestination::~WidgetDestination()
{
    delete ui;
}

void WidgetDestination::refreshDestinations(QString folder)
{
    ui->LabelFoldername->setText(folder);
    refreshDestinations();
}

void WidgetDestination::refreshDestinations()
{
    while (!this->ButtonList.isEmpty()) {
        delete this->ButtonList.takeLast();
    }

    // Get drive list
    QList<QStorageInfo> Storages = QStorageInfo::mountedVolumes();

    // Keep only valid destinations
    int x = 0;
    int y = 0;
    for (int i = 0; i < Storages.count(); i++) {
        QStorageInfo Storage = Storages.at(i);

        if (!Storage.isRoot() && !Storage.isReadOnly() && Storage.isValid() && Storage.isReady()) {
            QCommandLinkButton* Button = new QCommandLinkButton;
            Button->setCheckable(true);
            Button->setText(QDir(Storage.rootPath()).canonicalPath());
            Button->setDescription(Storage.name().isEmpty() ? " " : Storage.name());
            Button->setIcon(this->DestIcon);
            this->ButtonList << Button;

            connect(Button, &QCommandLinkButton::clicked, [this, Button]() {
                emit selectionChanged();
                Button->setIconSize(Button->isChecked() ? QSize(32, 32) : QSize(16, 16));
            });

            ui->GLayoutDestination->addWidget(Button, x, y, Qt::AlignCenter);
            if (++y == HZ_ITEM_COUNT) {
                y = 0;
                ++x;
            }
        }
    }

    // Adjust label depending on available storages
    ui->LabelNoDestAvailable->setVisible(this->ButtonList.isEmpty());
}

int WidgetDestination::selectedDestinations() const
{
    int Count = 0;
    for (int i = 0; i < this->ButtonList.count(); i++) {
        QCommandLinkButton* Button = this->ButtonList.at(i);
        if (Button->isChecked()) {
            Count++;
        }
    }
    return Count;
}

QList<QString> WidgetDestination::destinationList() const
{
    QList<QString> Destination;
    for (int i = 0; i < this->ButtonList.count(); i++) {
        QCommandLinkButton* Button = this->ButtonList.at(i);
        if (Button->isChecked()) {
            Destination << Button->text();
        }
    }
    return Destination;
}
