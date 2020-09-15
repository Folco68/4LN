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

#include "WindowAnalyze.hpp"
#include "Global.hpp"
#include "ThreadAnalyze.hpp"
#include "ui_WindowAnalyze.h"
#include <QDir>
#include <QPushButton>

WindowAnalyze::WindowAnalyze(int progresscount) : QWidget(nullptr), ui(new Ui::WindowAnalyze)
{
    ui->setupUi(this);
    ui->ProgressBar->setMaximum(1 + progresscount * 2); // +1: source directory. *2: 1 for parsing + 1 for action to take
    setWindowTitle(WINDOW_TITLE);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    setMinimumWidth(PROGRESS_WINDOW_MIN_WIDTH);

    connect(ThreadAnalyze::instance(), &ThreadAnalyze::parsingFile, this, &WindowAnalyze::parsingFile, Qt::QueuedConnection);
    connect(ThreadAnalyze::instance(), &ThreadAnalyze::parsingNextDirectory, this, &WindowAnalyze::parsingNextDirectory, Qt::QueuedConnection);
    connect(ui->ButtonCancel, &QPushButton::clicked, ThreadAnalyze::instance(), &ThreadAnalyze::requestInterruption, Qt::QueuedConnection);

    adjustSize();
}

WindowAnalyze::~WindowAnalyze()
{
    delete ui;
}

void WindowAnalyze::parsingNextDirectory()
{
    int Progress = 1 + ui->ProgressBar->value();
    ui->ProgressBar->setValue(Progress);
}

void WindowAnalyze::parsingFile(QString filename)
{
    QString format = QString("%1 (%p%)").arg(QDir::cleanPath(filename));
    ui->ProgressBar->setFormat(format);
}
