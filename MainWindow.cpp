#include "MainWindow.hpp"
#include "Global.hpp"
#include "ThreadAnalyze.hpp"
#include "ThreadClone.hpp"
#include "WidgetDestination.hpp"
#include "WindowDiff.hpp"
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

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // Setup UI
    ui->setupUi(this);
    setWindowTitle(WINDOW_TITLE);

    // Set the layout of the destinations box
    this->DestinationGrid = new QGridLayout(ui->DestinationWidget);

    // Create device list (will make UI consistent)
    refreshDestinations();

    // Connections
    connect(ui->ButtonRefresh, &QPushButton::clicked, [this]() { refreshDestinations(); });
    connect(ui->ButtonClone, &QPushButton::clicked, [this]() { clone(); });
    connect(ui->ButtonBrowse, &QPushButton::clicked, [this]() { browseSource(); });

    // End of analyze thread
    connect(ThreadAnalyze::instance(), &ThreadAnalyze::analyzeComplete, this, &MainWindow::analyzeComplete, Qt::QueuedConnection);
    connect(ThreadAnalyze::instance(), &ThreadAnalyze::finished, this, &MainWindow::deleteWindowAnalyze, Qt::QueuedConnection);

    // End of clone thread
    connect(ThreadClone::instance(), &ThreadClone::cloneComplete, this, &MainWindow::cloneComplete, Qt::QueuedConnection);
    connect(ThreadClone::instance(), &ThreadClone::finished, this, &MainWindow::deleteWindowClone, Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;

    while (!this->DestinationList.isEmpty()) {
        delete this->DestinationList.takeLast();
    }
}

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
            if (Column == HZ_DEST_MAX) {
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
        ui->EditDirectory->setText(QDir::cleanPath(QDir(Source).canonicalPath()));
        updateUI();
    }
}

//
//  updateUI
//
// Update destinations and "Clone" button
//

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

void MainWindow::analyzeComplete()
{
    WindowDiff* window = new WindowDiff;
    if (window->exec() == QDialog::Accepted) {
        this->WClone = new WindowClone(selectedDrivesCount());
        this->WClone->show();

        ThreadClone::instance()->clone();
    }

    delete window;
}

void MainWindow::deleteWindowAnalyze()
{
    this->WAnalyze->deleteLater();
}

void MainWindow::cloneComplete()
{
}

void MainWindow::deleteWindowClone()
{
    this->WClone->deleteLater();
}
