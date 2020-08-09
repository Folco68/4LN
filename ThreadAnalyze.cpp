#include "ThreadAnalyze.hpp"
#include "CopyData.hpp"
#include "Global.hpp"
#include <QDir>

ThreadAnalyze* ThreadAnalyze::threadanalyze = nullptr;

ThreadAnalyze* ThreadAnalyze::instance()
{
    if (threadanalyze == nullptr) {
        threadanalyze = new ThreadAnalyze;
    }
    return threadanalyze;
}

void ThreadAnalyze::release()
{
    delete threadanalyze;
    threadanalyze = nullptr;
}

void ThreadAnalyze::analyze(QString source, QList<QString> destinations)
{
    CopyData::instance()->init(source, destinations);
    start();
}

void ThreadAnalyze::run()
{
    // Parse source
    emit parsingNextDirectory();
    parseDirectory(CopyData::instance()->sourceDrive());

    // Parse destinations
    for (int i = 0; i < CopyData::instance()->destinationCount(); i++) {
        emit parsingNextDirectory();
        parseDirectory(CopyData::instance()->destinationDrive(i));
    }

    // Now parse every destination, comparing its content to the source
    // - default: file are flagged for deletion
    // - if a file is present but small, it's flagged for copy
    // - if a file is present and large enough, it will be ignored

    // Parse every destination, and compare with file data of every source file
    // i: drive index (destination)
    // j: file index in drive (destination)
    // k: file index (source)
    for (int i = 0; i < CopyData::instance()->destinationCount(); i++) {
        emit parsingNextDirectory();

        // Parse every file of a destination
        for (int j = 0; j < CopyData::instance()->destinationDrive(i)->fileCount(); j++) {
            FileData* destfile = CopyData::instance()->destinationDrive(i)->file(j);
            emit parsingFile(destfile->filename());

            // Compare to every file of the source
            for (int k = 0; k < CopyData::instance()->sourceDrive()->fileCount(); k++) {
                FileData* srcfile = CopyData::instance()->sourceDrive()->file(k);

                // First quick check, keep deletion flag if names mismatch
                if (srcfile->filename().compare(destfile->filename(), Qt::CaseInsensitive) != 0) {
                    continue;
                }

                // Else compare size, and don't copy if the files are large enough
                if ((srcfile->size() == destfile->size()) && (srcfile->size() > OVERWRITE_MAXIMUM_SIZE)) {
                    destfile->setProcess(IGNORE_FILE);
                }
                else {
                    destfile->setProcess(COPY_FILE);
                }
            }

            // Check if the user wants to cancel the analyze process
            if (isInterruptionRequested()) {
                return;
            }
        }
    }

    // Emit a signal depending on the thread termination
    if (!isInterruptionRequested()) {
        emit analyzeComplete();
    }
}

void ThreadAnalyze::parseDirectory(DriveData* drive, QString path)
{
    // Set directory sorting and filtering
    QDir dir(drive->basePath() + QDir::separator() + path);
    dir.setSorting(QDir::Name | QDir::DirsFirst);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    // Get all files/directories contained in the current folder
    QStringList strlist = dir.entryList();

    // Browse recursively the directories, else add the files to the files list
    for (int i = 0; i < strlist.count(); i++) {
        // Parse directories recursively, or add files to the list
        QString filename = QDir::cleanPath(dir.path() + QDir::separator() + strlist.at(i));
        emit parsingFile(filename);

        if (QFileInfo(filename).isDir()) {
            // Avoid useless separator if path is empty
            if (path.isEmpty()) {
                parseDirectory(drive, strlist.at(i));
            }
            else {
                parseDirectory(drive, path + QDir::separator() + strlist.at(i));
            }
        }
        else {
            if (path.isEmpty()) {
                FileData item(drive->basePath(), strlist.at(i), QFileInfo(filename).size());
                drive->addFile(item);
            }
            else {
                FileData item(drive->basePath(), path + QDir::separator() + strlist.at(i), QFileInfo(filename).size());
                drive->addFile(item);
            }
        }

        // Check if the user wants to cancel the analyze process
        if (isInterruptionRequested()) {
            return;
        }
    }
}
