#include "ThreadAnalyze.hpp"
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
    // Clean and init source container
    this->SourceItems.clear();
    this->SourceItems.init(source);

    // Clean and init destination containers
    this->DestinationsItems.clear();
    for (int i = 0; i < destinations.count(); i++) {
        this->DestinationsItems.append(ItemContainer(destinations.at(i)));
    }

    start();
}

void ThreadAnalyze::run()
{
    // Parse source
    emit parsingNextDirectory();
    parseDirectory(&this->SourceItems);

    // Parse destinations
    for (int i = 0; i < this->DestinationsItems.count(); i++) {
        emit parsingNextDirectory();
        parseDirectory(&this->DestinationsItems[i]);
    }

    // Now parse every destination, comparing its content to the source
    // - default: file are flagged for deletion
    // - if a file is present but small, it's flagged for copy
    // - if a file is present and large enough, it will be ignored

    // Parse every destination
    for (int i = 0; i < this->DestinationsItems.count(); i++) {
        emit parsingNextDirectory();

        // Parse every file of a destination
        for (int j = 0; j < this->DestinationsItems.at(i).count(); j++) {
            CopyItem* destination = this->DestinationsItems[i].getItem(j);

            emit parsingFile(destination->filename());

            // Compare to every file of the source
            for (int k = 0; k < this->SourceItems.count(); k++) {
                CopyItem* source = this->SourceItems.getItem(k);

                // First quick check, keep deletion flag if names mismatch
                if (source->filename().compare(destination->filename(), Qt::CaseInsensitive) != 0) {
                    continue;
                }

                // Else compare size, and don't copy if the files are large enough
                if ((source->size() == destination->size()) && (source->size() > OVERWRITE_MAXIMUM_SIZE)) {
                    destination->setProcess(IGNORE_FILE);
                }
                else {
                    destination->setProcess(COPY_FILE);
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

void ThreadAnalyze::parseDirectory(ItemContainer* container, QString path)
{
    // Set directory sorting and filtering
    QDir dir(container->basePath() + QDir::separator() + path);
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
                parseDirectory(container, strlist.at(i));
            }
            else {
                parseDirectory(container, path + QDir::separator() + strlist.at(i));
            }
        }
        else {
            if (path.isEmpty()) {
                CopyItem item(container->basePath(), strlist.at(i), QFileInfo(filename).size());
                container->addItem(item);
            }
            else {
                CopyItem item(container->basePath(), path + QDir::separator() + strlist.at(i), QFileInfo(filename).size());
                container->addItem(item);
            }
        }

        // Check if the user wants to cancel the analyze process
        if (isInterruptionRequested()) {
            return;
        }
    }
}
