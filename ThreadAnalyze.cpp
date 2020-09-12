/*
 * 4LN - Multiple Destinations Copier - batch copier to clone a content into several drives at once
 * Copyright (C) 2020 Martial Demolins AKA Folco
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * mail: martial <dot> demolins <at> gmail <dot> com
 */

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

        // Parse every file of the source
        for (int j = 0; j < CopyData::instance()->sourceDrive()->fileCount(); j++) {
            FileData* srcfile = CopyData::instance()->sourceDrive()->file(j);
            emit parsingFile(srcfile->filename());

            bool FileFound = false;

            // Compare to every file of every destination
            for (int k = 0; k < CopyData::instance()->destinationDrive(i)->fileCount(); k++) {
                FileData* destfile = CopyData::instance()->destinationDrive(i)->file(k);

                // First quick check, keep deletion flag if names mismatch
                if (srcfile->filename().compare(destfile->filename(), Qt::CaseInsensitive) != 0) {
                    continue;
                }

                // Name are equal. No additional check needed for a directory
                if ((srcfile->type() == TYPE_DIRECTORY) && (destfile->type() == TYPE_DIRECTORY)) {
                    destfile->setProcess(IGNORE_FILE);
                    FileFound = true;
                    break;
                }

                // Compare size, and don't copy the file if it's large enough
                if ((srcfile->size() == destfile->size()) && (srcfile->size() > OVERWRITE_MAXIMUM_SIZE)) {
                    destfile->setProcess(IGNORE_FILE);
                    FileFound = true;
                    break;
                }
                // Sizes mismatch or file is smaller than the limit, copy the source file
                // Note that we execute that if a file and aa directory have the same name => process is the good one
                else {
                    destfile->setProcess(COPY_FILE);
                    FileFound = true;
                    break;
                }
            }

            // The source file has not been found in the target, add it manually
            if (!FileFound) {
                FileData NewFile = *srcfile;
                NewFile.setProcess(COPY_FILE);
                CopyData::instance()->destinationDrive(i)->addFile(NewFile);
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
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

    // Get all files/directories contained in the current folder
    QStringList entrylist = dir.entryList();

    // Browse recursively the directories, else add the files to the files list
    for (int i = 0; i < entrylist.count(); i++) {
        // Parse directories recursively, or add files to the list
        QString filename = dir.path() + QDir::separator() + entrylist.at(i);
        emit parsingFile(filename);

        if (QFileInfo(filename).isDir()) {
            // This is a directory, add it to the list
            FileData item(path + QDir::separator() + entrylist.at(i), NO_SIZE, DELETE_FILE, TYPE_DIRECTORY);
            drive->addFile(item);

            // And parse it
            parseDirectory(drive, path + QDir::separator() + entrylist.at(i));
        }
        else {
            // This is a file
            FileData item(path + QDir::separator() + entrylist.at(i), QFileInfo(filename).size(), DELETE_FILE, TYPE_FILE);
            drive->addFile(item);
        }

        // Check if the user wants to cancel the analyze process
        if (isInterruptionRequested()) {
            return;
        }
    }
}
