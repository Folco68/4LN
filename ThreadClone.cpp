#include "ThreadClone.hpp"
#include "CopyData.hpp"
#include <QDir>
#include <QFile>

ThreadClone* ThreadClone::threadclone = nullptr;

ThreadClone* ThreadClone::instance()
{
    if (threadclone == nullptr) {
        threadclone = new ThreadClone;
    }
    return threadclone;
}

void ThreadClone::release()
{
    delete threadclone;
    threadclone = nullptr;
}

void ThreadClone::clone()
{
    start();
}

void ThreadClone::run()
{
    for (int i = 0; i < CopyData::instance()->destinationCount(); i++) {
        DriveData* drive = CopyData::instance()->destinationDrive(i);
        emit copyingNextDrive(drive->basePath());
        QList<FileData*> copylist;

        // Process is done in two passes:
        // - the first one delete the files
        // - the second one copy or update the remaining files
        // It increases free space to perform the copy

        // First pass:
        // - delete files flagged for deletion
        // - put in a new list files to be copied
        for (int j = 0; j < drive->fileCount(); j++) {
            FileData* file = drive->file(j);
            emit copyingFile(file->filename());

            FILE_PROCESS process = file->process();

            switch (process) {
                    // IGNORE: nothing to do
                case IGNORE_FILE:
                    break;

                    // COPY: add file data to the copy list
                case COPY_FILE:
                    copylist << file;
                    break;

                    // DELETE: erase file from current drive
                case DELETE_FILE:
                    QString filename = QString("%1%2%3").arg(drive->basePath()).arg(QDir::separator()).arg(file->filename());
                    if (file->type() == TYPE_FILE) {
                        QFile::remove(filename);
                    }
                    else {
                        QDir(filename).removeRecursively();
                    }
            }
        }

        // Second pass:
        // - delete destination file if it already exists
        // - copy the source file
        for (int k = 0; k < copylist.count(); k++) {
            QString src  = QString("%1%2%3").arg(CopyData::instance()->sourceDrive()->basePath()).arg(QDir::separator()).arg(copylist.at(k)->filename());
            QString dest = QString("%1%2%3").arg(drive->basePath()).arg(QDir::separator()).arg(copylist.at(k)->filename());
            emit copyingFile(dest);

            // Source file won't be copied if destination already exists, so we have to remove it first
            // A directory won't never been flagged as "to be copied" if it already exists in the destination (no size limit or mismatch)
            if (QFile::exists(dest) && (copylist.at(k)->type() == TYPE_FILE)) {
                QFile::remove(dest);
            }

            // Perform the copy, depending on the type: file or directory
            if (copylist.at(k)->type() == TYPE_FILE) {
                QFile::copy(src, dest);
            }
            else {
                QString newdir = QDir(dest).dirName(); // Name of the directory to create
                QDir dir(dest);                        // Full path of the directory to create
                dir.cdUp();                            // One dir up
                dir.mkdir(newdir);                     // Then perform the creation
            }
        }
    }
}
