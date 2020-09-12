#include "SourceBox.hpp"
#include <QFileInfo>
#include <QMimeData>

//
//  dragEnterEvent
//
// Tell the SourceBox to accept only one directory
//

void SourceBox::dragEnterEvent(QDragEnterEvent* event)
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
// Send a signal caught by the MainWindow
//

void SourceBox::dropEvent(QDropEvent* event)
{
    emit directoryDropped(event->mimeData()->urls().at(0).toLocalFile());
}
