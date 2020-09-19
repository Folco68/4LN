#include "WidgetDestination.hpp"
#include "ui_WidgetDestination.h"
#include <QLayoutItem>
#include <QStorageInfo>

WidgetDestination::WidgetDestination(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::WidgetDestination)
{
    ui->setupUi(this);

    this->DestIcon = QIcon(":/pic/USBstick.png");
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
