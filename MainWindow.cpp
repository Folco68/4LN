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

#include "MainWindow.hpp"
#include "CopyData.hpp"
#include "Global.hpp"
#include "ThreadAnalyze.hpp"
#include "ThreadClone.hpp"
#include "WindowDiff.hpp"
#include "WindowHelp.hpp"
#include "ui_MainWindow.h"
#include <QBoxLayout>
#include <QFileInfo>
#include <QLayoutItem>
#include <QMessageBox>

MainWindow::MainWindow(QString directory) : ui(new Ui::MainWindow), MainWindowStep(STEP_ZERO)
{
    // Setup UI
    ui->setupUi(this);
    ui->ButtonHelp->setStyleSheet("Background: yellow");

    // Setup window
    setWindowTitle(WINDOW_TITLE);
    setMinimumSize(MAIN_WINDOW_MIN_WIDTH, MAIN_WINDOW_MIN_HEIGHT);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    // Prepare central widgets
    this->DropBox        = new WidgetDrop;
    this->DestinationBox = new WidgetDestination;
    this->OverwriteBox   = new WidgetOverwrite;

    QBoxLayout* layout = reinterpret_cast<QBoxLayout*>(centralWidget()->layout());
    layout->insertWidget(0, this->DropBox);
    layout->insertWidget(0, this->DestinationBox);
    layout->insertWidget(0, this->OverwriteBox);

    // Button connections
    connect(ui->ButtonHelp, &QPushButton::clicked, [this]() { WindowHelp::openWindowHelp(this); });
    connect(ui->ButtonPrevious, &QPushButton::clicked, [this]() { changeStep(this->MainWindowStep - 1); });
    connect(ui->ButtonNext, &QPushButton::clicked, [this]() { changeStep(this->MainWindowStep + 1); });
    connect(ui->ButtonRefresh, &QPushButton::clicked, [this]() { this->DestinationBox->refreshDestinations(); });

    // Connections to analyze thread
    connect(ThreadAnalyze::instance(), &ThreadAnalyze::finished, this, &MainWindow::deleteWindowAnalyze, Qt::QueuedConnection);
    connect(ThreadAnalyze::instance(), &ThreadAnalyze::analyzeComplete, this, &MainWindow::analyzeComplete, Qt::QueuedConnection);
    connect(ThreadAnalyze::instance(), &ThreadAnalyze::analyzeAborted, this, &MainWindow::threadAborted, Qt::QueuedConnection);

    // Connections to clone thread
    connect(ThreadClone::instance(), &ThreadClone::finished, this, &MainWindow::deleteWindowClone, Qt::QueuedConnection);
    connect(ThreadClone::instance(), &ThreadClone::cloneComplete, this, &MainWindow::cloneComplete, Qt::QueuedConnection);
    connect(ThreadClone::instance(), &ThreadClone::cloneAborted, this, &MainWindow::threadAborted, Qt::QueuedConnection);

    // Connection with main widget modules
    connect(this->DropBox, &WidgetDrop::directoryDropped, [this](QString directory) {
        this->SourceDirectory = directory;
        changeStep(STEP_SELECT_DEST);
    });

    connect(this->DestinationBox, &WidgetDestination::selectionChanged, [this]() { updateUI(); });

    // Handle argument passed to the program through the OS
    if (QFileInfo(directory).isDir()) {
        this->SourceDirectory = directory;
        changeStep(STEP_SELECT_DEST);
    }
    // Else default to drop box
    else {
        changeStep(STEP_DROP);
    }

    adjustSize();
}

MainWindow::~MainWindow()
{
    delete this->DropBox;
    delete this->DestinationBox;
    delete ui;
}

//  changeStep
//
// Update the central widget of the main window
//
void MainWindow::changeStep(int mws)
{
    if (mws == STEP_SELECT_DEST) {
        this->DestinationBox->refreshDestinations(this->SourceDirectory);
    }

    if (mws == STEP_CLONE) {
        this->WAnalyze = new WindowAnalyze(this->DestinationBox->selectedDestinations());
        this->WAnalyze->show();
        setVisible(false);
        ThreadAnalyze::instance()->analyze(this->SourceDirectory, this->DestinationBox->destinationList(), this->OverwriteBox->value());
    }

    this->MainWindowStep = mws;
    updateUI();
}

void MainWindow::updateUI()
{
    int mws = this->MainWindowStep;

    this->DropBox->setVisible(mws == STEP_DROP);
    this->DestinationBox->setVisible(mws == STEP_SELECT_DEST);
    this->OverwriteBox->setVisible(mws == STEP_OVERWRITE);
    ui->ButtonPrevious->setEnabled(mws != STEP_DROP);
    ui->ButtonNext->setDisabled((mws == STEP_DROP) || ((mws == STEP_SELECT_DEST) && (this->DestinationBox->selectedDestinations() == 0)));
    ui->ButtonRefresh->setVisible(mws == STEP_SELECT_DEST);

    adjustSize();
}

void MainWindow::threadAborted()
{
    setVisible(true);
    changeStep(STEP_OVERWRITE);
}

//  analyzeComplete
//
// Called at the end of analyzis process. Display a diff window, asking a confirmation before starting the cloning process
//
void MainWindow::analyzeComplete()
{
    WindowDiff* window = new WindowDiff;
    // Start the new clone thread if the dialog was accetped
    if (window->exec() == QDialog::Accepted) {
        this->WClone = new WindowClone(CopyData::instance()->filesCount());
        this->WClone->show();
        ThreadClone::instance()->start();
    }
    else {
        setVisible(true);
        changeStep(STEP_OVERWRITE);
    }

    delete window;
}

void MainWindow::deleteWindowAnalyze()
{
    this->WAnalyze->deleteLater();
}

//  cloneComplete
//
// Called at the end of clone process. Display a dialog and refresh the drive list (which unselect them)
//
void MainWindow::cloneComplete()
{
    changeStep(STEP_DROP);
    setVisible(true);
    QMessageBox::information(this, WINDOW_TITLE, tr("Cloning process complete."), QMessageBox::Ok);
}

void MainWindow::deleteWindowClone()
{
    this->WClone->deleteLater();
}
