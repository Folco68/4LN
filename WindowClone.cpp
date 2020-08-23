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
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setMinimumWidth(PROGRESS_WINDOW_MIN_WIDTH);

    connect(ThreadClone::instance(), &ThreadClone::copyingFile, this, &WindowClone::copyingFile, Qt::QueuedConnection);
    connect(ThreadClone::instance(), &ThreadClone::copyingNextDrive, this, &WindowClone::copyingNextDrive, Qt::QueuedConnection);
    connect(ui->ButtonCancel, &QPushButton::clicked, ThreadClone::instance(), &ThreadClone::requestInterruption, Qt::QueuedConnection);

    adjustSize();
}

WindowClone::~WindowClone()
{
    delete ui;
}

void WindowClone::copyingNextDrive(QString drivename)
{
    int Progress = ui->ProgressBar->value() + 1;
    ui->ProgressBar->setValue(Progress);

    ui->LabelCloning->setText(tr("Cloning... (%1)").arg(drivename));
}

void WindowClone::copyingFile(QString filename)
{
    QString format = QDir::cleanPath(filename);
    ui->ProgressBar->setFormat(format);
}
