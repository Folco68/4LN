#include "ThreadAnalyze.hpp"

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

void ThreadAnalyze::analyze(QString source, QList<Destination*> destinations)
{
    this->SourcePath      = source;
    this->DestinationList = destinations;
    start();
}

void ThreadAnalyze::run()
{
    // Clear existing data
    this->SourceItems.clear();
    this->DestinationItems.clear();

    // Parse source
    emit parsingNextDirectory();
    parseDirectory(this->SourcePath, &this->SourceItems);

    // Parse destinations
    for (int i = 0; i < this->DestinationList.count(); i++) {
        emit parsingNextDirectory();

        QList<CopyItem> items;
        parseDirectory(this->DestinationList.at(i)->drivePath(), &items);
        this->DestinationItems << items;
    }

    // Emit a signal depending on the thread termination
    if (isInterruptionRequested()) {
        emit analyzeCanceled();
    }
    else {
        emit analyzeComplete();
    }
}

void ThreadAnalyze::parseDirectory(QString directory, QList<CopyItem>* list)
{
    // Set directory sorting and filtering
    QDir dir(directory);
    dir.setSorting(QDir::Name | QDir::DirsFirst);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    // Get all files/directories contained in the current folder
    QStringList strlist = dir.entryList();

    // Browse recursively the directories, else add the files to the files list
    for (int i = 0; i < strlist.count(); i++) {
        // Parse directories recursively, or add files to the list
        QString filename = directory + strlist.at(i);
        emit parsingFile(filename);

        if (QFileInfo(filename).isDir()) {
            parseDirectory(filename + QDir::separator(), list);
        }
        else {
            CopyItem item(filename, QFileInfo(filename).size());
            *list << item;
        }

        // Check if the user wants to cancel the analyze process
        if (isInterruptionRequested()) {
            return;
        }
    }
}
