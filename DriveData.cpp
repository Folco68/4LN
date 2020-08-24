/*
 * MDC - Multiple Destinations Copier - batch copier to clone a content into several drives at once
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

#include "DriveData.hpp"
#include <QDir>

DriveData::DriveData(QString basepath) : BasePath(QDir::cleanPath(basepath))
{
}

//  basePath
//
// Return the base path of the drive
//
QString DriveData::basePath() const
{
    return this->BasePath;
}

//  fileCount
//
// Return the number of files+directories contained by the drive
//
int DriveData::fileCount() const
{
    return this->FileList.count();
}

//  file
//
// Return the file with index "index"
//
FileData* DriveData::file(int index)
{
    return &this->FileList[index];
}

//  addFile
//
// Add a file to the the list
//
void DriveData::addFile(FileData file)
{
    this->FileList.append(file);
}

//  filesMarkedForDeletion
//
// Return the list of files flagged for deletion. Used by the diff window
//
QList<FileData> DriveData::filesMarkedForDeletion() const
{
    QList<FileData> list;
    for (int i = 0; i < this->FileList.count(); i++) {
        if (this->FileList.at(i).process() == DELETE_FILE) {
            list << this->FileList.at(i);
        }
    }
    return list;
}

//  filesMarkedForCopy
//
// Return the list of files flagged for copy. Used by the diff window
//
QList<FileData> DriveData::filesMarkedForCopy() const
{
    QList<FileData> list;
    for (int i = 0; i < this->FileList.count(); i++) {
        if (this->FileList.at(i).process() == COPY_FILE) {
            list << this->FileList.at(i);
        }
    }
    return list;
}
