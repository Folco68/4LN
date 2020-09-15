/*
 * 4LN - batch copier to clone a content into several drives at once
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

#include "CopyData.hpp"

CopyData* CopyData::copydata = nullptr;

CopyData* CopyData::instance()
{
    if (copydata == nullptr) {
        copydata = new CopyData;
    }
    return copydata;
}

void CopyData::release()
{
    delete copydata;
}

CopyData::~CopyData()
{
    clear();
    copydata = nullptr;
}

//  clear
//
// Core of the delete operation. Also used when initializing the object
//
void CopyData::clear()
{
    // Delete source
    delete this->SourceDrive;
    this->SourceDrive = nullptr;

    // Delete destinations
    while (!this->DestinationDrives.isEmpty()) {
        delete this->DestinationDrives.takeLast();
    }
}

//  init
//
// Initialize the object with UI data. Needed because this is a singleton, so there is no public constructor
//
void CopyData::init(QString source, QList<QString> destinations, int overwrite)
{
    clear();

    this->SourceDrive = new DriveData(source);

    for (int i = 0; i < destinations.count(); i++) {
        DriveData* drive = new DriveData(destinations.at(i));
        this->DestinationDrives.append(drive);
    }

    this->OverwriteSize = overwrite;
}

//  sourceDrive
//
// Return the source drive/path
//
DriveData* CopyData::sourceDrive()
{
    return this->SourceDrive;
}

//  clear
//
// Return the drive with index "index". No check is done
//
DriveData* CopyData::destinationDrive(int index)
{
    return this->DestinationDrives.at(index);
}

//  destinationCount
//
// Return the number of destination drives
//
int CopyData::destinationCount() const
{
    return this->DestinationDrives.count();
}

//  filesCount
//
// Return the count of files of all destinations
//
int CopyData::filesCount() const
{
    int count = 0;
    for (int i = 0; i < this->DestinationDrives.count(); i++) {
        count += this->DestinationDrives.at(i)->fileCount();
    }
    return count;
}

//  overwrite
//
// Return the maximum size of overwritten files
//
int CopyData::overwriteSize() const
{
    return this->OverwriteSize;
}
