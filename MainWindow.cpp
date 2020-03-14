#include "MainWindow.hpp"
#include "Destination.hpp"
#include "Global.hpp"
#include "ui_MainWindow.h"
#include <QBoxLayout>
#include <QCheckBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QStorageInfo>
#include <QString>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Setup UI
    ui->setupUi(this);
    setWindowTitle(MAIN_WINDOW_TITLE);

    // Set the layout of the destinations box
    this->DestinationGrid = new QGridLayout(ui->DestinationWidget);

    // Create device list (will make UI consistent)
    refreshDeviceList();

    // Connections
    connect(ui->ButtonDevicesStatus, &QPushButton::clicked, [this]() { displayDeviceStatus(); });
    connect(ui->ButtonRefresh, &QPushButton::clicked, [this]() { refreshDeviceList(); });
    connect(ui->ButtonClone, &QPushButton::clicked, [this]() { clone(); });
    connect(ui->ButtonBrowse, &QPushButton::clicked, [this]() { browseSource(); });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshDeviceList()
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

    for (int i = 0; i < Storages.count(); i++, Column++) {
        QStorageInfo Storage = Storages.at(i);

        // Add only valid/available/RW sources
        if (!Storage.isRoot() && !Storage.isReadOnly() && Storage.isValid() && Storage.isReady()) {
            Device = new Destination(Storage);
            this->DestinationList << Device;
        }

        this->DestinationGrid->addWidget(Device, Row, Column);

        // Create another line if destination line is full
        if (Column == HZ_DEVICE_COUNT - 1) {
            Column = 0;
            Row++;
        }
    }

    adjustSize();
    updateUI();
}

void MainWindow::displayDeviceStatus()
{
}

void MainWindow::clone() const
{
}

void MainWindow::browseSource()
{
    QString Directory = ui->EditDirectory->text();
    QString Source    = QFileDialog::getExistingDirectory(this, tr("Select source directory"), Directory);
    if (!Source.isEmpty()) {
        ui->EditDirectory->setText(Source);
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
}
