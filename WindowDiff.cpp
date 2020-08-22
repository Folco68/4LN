#include "WindowDiff.hpp"
#include "CopyData.hpp"
#include "Global.hpp"
#include "ui_WindowDiff.h"
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QString>
#include <QVBoxLayout>

WindowDiff::WindowDiff(QWidget *parent) :
      QDialog(parent),
      ui(new Ui::WindowDiff)
{
    ui->setupUi(this);
    setWindowTitle(WINDOW_TITLE);

    // Buttons
    connect(ui->ButtonClone, &QPushButton::clicked, [this]() { accept(); });
    connect(ui->ButtonCancel, &QPushButton::clicked, [this]() { reject(); });

    // Source label
    ui->LabelSource->setText(tr("Source directory: %1").arg(CopyData::instance()->sourceDrive()->basePath()));

    QVBoxLayout* vlayout = new QVBoxLayout;

    // Create and insert a widget for each destination
    for (int i = 0; i < CopyData::instance()->destinationCount(); i++) {
        DriveData* drive = CopyData::instance()->destinationDrive(i);

        // Get the files to process
        QList<FileData> copylist   = drive->filesMarkedForCopy();
        QList<FileData> deletelist = drive->filesMarkedForDeletion();

        // Groupbox displaying infos for every drive
        QGroupBox* groupbox = new QGroupBox(drive->basePath());
        groupbox->setLayout(new QVBoxLayout);
        vlayout->addWidget(groupbox);

        // Maybe there is nothing to do for this device?
        if (copylist.isEmpty() && deletelist.isEmpty()) {
            groupbox->layout()->addWidget(new QLabel(tr("This destination is up to date")));
        }

        // Some files are to be copied or deleted
        else {
            // Files to be copied
            if (!copylist.isEmpty()) {
                groupbox->layout()->addWidget(new QLabel(tr("Files to be copied:")));
                for (int i = 0; i < copylist.count(); i++) {
                    groupbox->layout()->addWidget(new QLabel(copylist.at(i).filename()));
                }
            }

            // Insert separation line if there is files to be copied and deleted
            if (!copylist.isEmpty() && !deletelist.isEmpty()) {
                groupbox->layout()->addWidget(new QLabel(""));
            }

            // Files to be deleted
            if (!deletelist.isEmpty()) {
                groupbox->layout()->addWidget(new QLabel(tr("Files to be deleted:")));
                for (int i = 0; i < deletelist.count(); i++) {
                    groupbox->layout()->addWidget(new QLabel(deletelist.at(i).filename()));
                }
            }
        }
    }

    vlayout->addItem(new QSpacerItem(0, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
    QWidget* widget = new QWidget;
    widget->setLayout(vlayout);
    ui->ScrollArea->setWidget(widget);

    // Finally, adjust window size
    adjustSize();
}

WindowDiff::~WindowDiff()
{
    delete ui;
}
