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

#include "MainWindow.hpp"
#include "../Core/CopyData.hpp"
#include "../Core/ThreadAnalyze.hpp"
#include "../Core/ThreadClone.hpp"
#include "../Global.hpp"
#include "ui_MainWindow.h"
#include "WindowDiff.hpp"
#include "WindowHelp.hpp"
#include <QBoxLayout>
#include <QFileInfo>
#include <QLayoutItem>
#include <QMessageBox>

MainWindow::MainWindow(QString directory)
    : ui(new Ui::MainWindow)
    , MainWindowStep(STEP_ZERO)
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

    QBoxLayout* Layout = reinterpret_cast<QBoxLayout*>(centralWidget()->layout());
    Layout->insertWidget(0, this->DropBox);
    Layout->insertWidget(0, this->DestinationBox);
    Layout->insertWidget(0, this->OverwriteBox);

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
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

MainWindow::~MainWindow()
{
    delete this->DropBox;
    delete this->DestinationBox;
    delete ui;

    // Release singletons here and not in main(), because QGuiApplication does not return on all platforms
    ThreadAnalyze::release();
    ThreadClone::release();
    CopyData::release();
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
    int MainWindowStep = this->MainWindowStep;

    // First, set all boxes invisible, to avoid a vertical expansion of the main window
    // (adjustUI() may increase size, but not reduce it)
    this->DropBox->setVisible(false);
    this->DestinationBox->setVisible(MainWindowStep == false);
    this->OverwriteBox->setVisible(MainWindowStep == false);

    // Then make a box visible, depending on the program state
    this->DropBox->setVisible(MainWindowStep == STEP_DROP);
    this->DestinationBox->setVisible(MainWindowStep == STEP_SELECT_DEST);
    this->OverwriteBox->setVisible(MainWindowStep == STEP_OVERWRITE);

    // Handle buttons
    ui->ButtonPrevious->setEnabled(MainWindowStep != STEP_DROP);
    ui->ButtonNext->setDisabled((MainWindowStep == STEP_DROP) || ((MainWindowStep == STEP_SELECT_DEST) && (this->DestinationBox->selectedDestinations() == 0)));
    ui->ButtonRefresh->setVisible(MainWindowStep == STEP_SELECT_DEST);

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
    WindowDiff* Window = new WindowDiff;
    // Start the new clone thread if the dialog was accetped
    if (Window->exec() == QDialog::Accepted) {
        this->WClone = new WindowClone(CopyData::instance()->filesCount());
        this->WClone->show();
        this->CloneTimer.start();
        ThreadClone::instance()->start();
    }
    else {
        setVisible(true);
        changeStep(STEP_OVERWRITE);
    }

    delete Window;
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
    int Elapsed = this->CloneTimer.elapsed() / 1000; // Get rid of ms
    int h       = Elapsed / (60 * 60);
    Elapsed -= h * 60 * 60;
    int mn = Elapsed / 60;
    int s  = Elapsed % 60;

    setVisible(true);
    changeStep(STEP_DROP);
    QMessageBox::information(this, WINDOW_TITLE, tr("Cloning process completed in %1h %2mn %3s").arg(h).arg(mn).arg(s), QMessageBox::Ok);
}

void MainWindow::deleteWindowClone()
{
    this->WClone->deleteLater();
}
