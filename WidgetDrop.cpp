#include "WidgetDrop.hpp"
#include "ui_WidgetDrop.h"
#include <QFileInfo>
#include <QList>
#include <QMimeData>
#include <QUrl>

WidgetDrop::WidgetDrop(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::WidgetDrop)
{
    ui->setupUi(this);
}

WidgetDrop::~WidgetDrop()
{
    delete ui;
}

//
//  dragEnterEvent
//
// Tell the SourceBox to accept only one directory
//

void WidgetDrop::dragEnterEvent(QDragEnterEvent* event)
{
    // We want only URLs
    if (!event->mimeData()->hasUrls()) {
        return;
    }

    // We want only one URL
    QList<QUrl> list = event->mimeData()->urls();
    if (list.count() != 1) {
        return;
    }

    // We want only a directory
    QString Directory = list.at(0).toLocalFile();
    if (!QFileInfo(Directory).isDir()) {
        return;
    }

    event->acceptProposedAction();
}

//
//  dropEvent
//
// Display the selected directory and send a signal caught by the MainWindow
//

void WidgetDrop::dropEvent(QDropEvent* event)
{
    QString directory = event->mimeData()->urls().at(0).toLocalFile();
    emit directoryDropped(directory);
}
