/*
 * 4LN - Multiple Destinations Copier - batch copier to clone a content into several drives at once
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

#include "WindowClone.hpp"
#include "Global.hpp"
#include "ThreadClone.hpp"
#include "ui_WindowClone.h"
#include <QDir>

WindowClone::WindowClone(int progresscount) : ui(new Ui::WindowClone)
{
    ui->setupUi(this);
    ui->ProgressBar->setMaximum(progresscount);
    setWindowTitle(WINDOW_TITLE);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    setMinimumWidth(PROGRESS_WINDOW_MIN_WIDTH);

    connect(ThreadClone::instance(), &ThreadClone::copyingFile, this, &WindowClone::copyingFile, Qt::QueuedConnection);
    connect(ThreadClone::instance(), &ThreadClone::copyingNextDrive, this, &WindowClone::copyingNextDrive, Qt::QueuedConnection);
    connect(ThreadClone::instance(), &ThreadClone::updateCount, this, &WindowClone::updateCount, Qt::QueuedConnection);
    connect(ui->ButtonCancel, &QPushButton::clicked, ThreadClone::instance(), &ThreadClone::requestInterruption, Qt::QueuedConnection);

    adjustSize();
}

WindowClone::~WindowClone()
{
    delete ui;
}

void WindowClone::copyingNextDrive(QString drivename)
{
    ui->LabelCloning->setText(tr("Cloning... (%1)").arg(drivename));
}

void WindowClone::copyingFile(QString filename)
{
    int Progress = ui->ProgressBar->value() + 1;
    ui->ProgressBar->setValue(Progress);

    QString format = QDir::cleanPath(filename);
    ui->ProgressBar->setFormat(format);
}

void WindowClone::updateCount(int count)
{
    int CurrentMax = ui->ProgressBar->maximum();
    ui->ProgressBar->setMaximum(CurrentMax + count);
}
