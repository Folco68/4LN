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

QString DriveData::basePath() const
{
    return this->BasePath;
}

int DriveData::fileCount() const
{
    return this->FileList.count();
}

FileData* DriveData::file(int index)
{
    return &this->FileList[index];
}

void DriveData::addFile(FileData file)
{
    this->FileList.append(file);
}

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
