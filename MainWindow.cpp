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
#include "SourceBox.hpp"
#include "ThreadAnalyze.hpp"
#include "ThreadClone.hpp"
#include "WidgetDestination.hpp"
#include "WindowDiff.hpp"
#include "WindowHelp.hpp"
#include "ui_MainWindow.h"
#include <QBoxLayout>
#include <QCheckBox>
#include <QDir>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QSpacerItem>
#include <QStorageInfo>
#include <QString>
#include <QVBoxLayout>

MainWindow::MainWindow(QString directory) : ui(new Ui::MainWindow)
{
    // Setup UI
    ui->setupUi(this);
    setWindowTitle(WINDOW_TITLE);
    ui->ButtonHelp->setStyleSheet("Background: yellow");

    // Set the layout of the destinations box
    this->DestinationGrid = new QGridLayout(ui->DestinationWidget);

    // Create device list (will make UI consistent)
    refreshDestinations();

    // UI connections
    connect(ui->ButtonRefresh, &QPushButton::clicked, [this]() { refreshDestinations(); });
    connect(ui->ButtonClone, &QPushButton::clicked, [this]() { clone(); });
    connect(ui->ButtonBrowse, &QPushButton::clicked, [this]() { browseSource(); });
    connect(ui->ButtonHelp, &QPushButton::clicked, [this]() { WindowHelp::openWindowHelp(this); });
    connect(ui->BoxSource, &SourceBox::directoryDropped, [this](QString directory) { setSource(directory); });

    // Connections to analyze thread
    connect(ThreadAnalyze::instance(), &ThreadAnalyze::started, this, &MainWindow::makeInvisible, Qt::QueuedConnection);
    connect(ThreadAnalyze::instance(), &ThreadAnalyze::finished, this, &MainWindow::deleteWindowAnalyze, Qt::QueuedConnection);
    connect(ThreadAnalyze::instance(), &ThreadAnalyze::finished, this, &MainWindow::makeVisible, Qt::QueuedConnection);
    connect(ThreadAnalyze::instance(), &ThreadAnalyze::analyzeComplete, this, &MainWindow::analyzeComplete, Qt::QueuedConnection);

    // Connections to clone thread
    connect(ThreadClone::instance(), &ThreadClone::started, this, &MainWindow::makeInvisible, Qt::QueuedConnection);
    connect(ThreadClone::instance(), &ThreadClone::finished, this, &MainWindow::deleteWindowClone, Qt::QueuedConnection);
    connect(ThreadClone::instance(), &ThreadClone::finished, this, &MainWindow::makeVisible, Qt::QueuedConnection);
    connect(ThreadClone::instance(), &ThreadClone::cloneComplete, this, &MainWindow::cloneComplete, Qt::QueuedConnection);

    // Handles argument passed to the program through the OS
    if (QFileInfo(directory).isDir()) {
        setSource(directory);
    }
}

MainWindow::~MainWindow()
{
    delete ui;

    while (!this->DestinationList.isEmpty()) {
        delete this->DestinationList.takeLast();
    }
}

//  makeVisible
//
// Make the main window visible again, after a thread has completed
//
void MainWindow::makeVisible()
{
    setVisible(true);
}

void MainWindow::makeInvisible()
{
    setVisible(false);
}

//  refreshDestinations
//
// Update the list drives available for cloning
//
void MainWindow::refreshDestinations()
{
    // Clear previous destinations
    while (!this->DestinationList.isEmpty()) {
        delete this->DestinationList.takeLast();
    }

    // Create the new destinations
    QList<QStorageInfo> Storages = QStorageInfo::mountedVolumes();
    WidgetDestination* Device    = nullptr;
    int Row                      = 0;
    int Column                   = 0;

    for (int i = 0; i < Storages.count(); i++) {
        QStorageInfo Storage = Storages.at(i);

        // Add only !root/RW/valid/ready devices
        if (!Storage.isRoot() && !Storage.isReadOnly() && Storage.isValid() && Storage.isReady()) {
            Device = new WidgetDestination(Storage);
            this->DestinationList << Device;

            // Add the widget to the UI, and create another line if destination line is full
            this->DestinationGrid->addWidget(Device, Row, Column);
            if (Column == HZ_DEST_MAX - 1) {
                Column = 0;
                Row++;
            }
            else {
                Column++;
            }

            // Refresh UI when a checkbox is toggled, to enable/disable the Clone button
            connect(Device->checkBox(), &QCheckBox::stateChanged, [this]() { updateUI(); });
        }
    }

    updateUI();
}

//  clone
//
// Start the analyze process
//
void MainWindow::clone()
{
    // Create and show the progress window
    this->WAnalyze = new WindowAnalyze(selectedDrivesCount());
    this->WAnalyze->show();

    // Start the analyzer thread
    ThreadAnalyze::instance()->analyze(ui->EditDirectory->text(), selectedDrives());
}

void MainWindow::browseSource()
{
    // Default directory
    QString directory = ui->EditDirectory->text();
    if (!QDir(directory).exists()) {
        directory = QDir::homePath();
    }

    // Open directory selection
    QString Source = QFileDialog::getExistingDirectory(this, tr("Select source directory"), directory);
    if (!Source.isEmpty()) {
        setSource(Source);
    }
}

//  setSource
//
// Called by the browseSource method, or when the program is run by dropping a directory on its icon
//
void MainWindow::setSource(QString directory)
{
    ui->EditDirectory->setText(QDir::cleanPath(QDir(directory).canonicalPath()));
    updateUI();
}

//
//  updateUI
//
// Update destinations and "Clone" button
void MainWindow::updateUI()
{
    // Shortcuts
    bool SrcDefined    = !ui->EditDirectory->text().isEmpty();
    bool DestAvailable = !this->DestinationList.isEmpty();
    bool DestSelected  = selectedDrivesCount() != 0;

    // Adjust UI
    ui->LabelNoDestination->setVisible(!DestAvailable);
    ui->DestinationWidget->setVisible(DestAvailable);
    ui->ButtonClone->setEnabled(SrcDefined && DestSelected);

    // and window size
    adjustSize();
}

//  selectedDrives
//
// Return the list of the drives selected by the user
//
QList<QString> MainWindow::selectedDrives() const
{
    QList<QString> Drives;
    for (int i = 0; i < this->DestinationList.count(); i++) {
        if (DestinationList.at(i)->isSelected()) {
            Drives << DestinationList.at(i)->drivePath();
        }
    }

    return Drives;
}

//  selectedDriveCount
//
// Return the count of selected drives
//
int MainWindow::selectedDrivesCount() const
{
    int count = 0;
    for (int i = 0; i < this->DestinationList.count(); i++) {
        if (DestinationList.at(i)->isSelected()) {
            count++;
        }
    }

    return count;
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
    QMessageBox::information(this, WINDOW_TITLE, tr("Cloning process complete."), QMessageBox::Ok);
    refreshDestinations();
}

void MainWindow::deleteWindowClone()
{
    this->WClone->deleteLater();
}
