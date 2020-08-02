#include "MainWindow.hpp"
#include "Destination.hpp"
#include "Global.hpp"
#include "ThreadAnalyze.hpp"
#include "WindowAnalyze.hpp"
#include "ui_MainWindow.h"
#include <QBoxLayout>
#include <QCheckBox>
#include <QDir>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
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
    Destination* Device          = nullptr;
    int Row                      = 0;
    int Column                   = 0;

    for (int i = 0; i < Storages.count(); i++) {
        QStorageInfo Storage = Storages.at(i);

        // Add only !root/RW/valid/ready devices
        if (!Storage.isRoot() && !Storage.isReadOnly() && Storage.isValid() && Storage.isReady()) {
            Device = new Destination(Storage);
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
    // Create and show the progress window (modal window), passing the count of analyze steps
    this->Window = new WindowAnalyze(1 + selectedDestinations().count());
    this->Window->show();

    // End of thread handling
    connect(ThreadAnalyze::instance(), &ThreadAnalyze::analyzeComplete, this, &MainWindow::analyzeComplete, Qt::QueuedConnection);
    connect(ThreadAnalyze::instance(), &ThreadAnalyze::analyzeCanceled, this, &MainWindow::analyzeCanceled, Qt::QueuedConnection);
    connect(ThreadAnalyze::instance(), &ThreadAnalyze::finished, this, &MainWindow::deleteWindowAnalyze, Qt::QueuedConnection);

    // Finally, start the analyzer thread
    ThreadAnalyze::instance()->analyze(ui->EditDirectory->text(), selectedDestinations());
}

void MainWindow::browseSource()
{
    // Default directory
    QString directory = ui->EditDirectory->text();
    if (directory.isEmpty()) {
        directory = QDir::homePath();
    }

    // Open directory selection
    QString Source = QFileDialog::getExistingDirectory(this, tr("Select source directory"), directory);
    if (!Source.isEmpty()) {
        ui->EditDirectory->setText(Source + QDir::separator());
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
    bool DestSelected  = !selectedDestinations().isEmpty();

    // Adjust UI
    ui->LabelNoDestination->setVisible(!DestAvailable);
    ui->DestinationWidget->setVisible(DestAvailable);
    ui->ButtonClone->setEnabled(SrcDefined && DestSelected);

    // and window size
    adjustSize();
}

QList<Destination*> MainWindow::selectedDestinations() const
{
    QList<Destination*> List;
    for (int i = 0; i < this->DestinationList.count(); i++) {
        Destination* dest = this->DestinationList.at(i);
        if (dest->isSelected()) {
            List << dest;
        }
    }
    return List;
}

void MainWindow::analyzeComplete()
{
}

void MainWindow::analyzeCanceled()
{
}

void MainWindow::deleteWindowAnalyze()
{
    this->Window->deleteLater();
}
