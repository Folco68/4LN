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

void CopyData::init(QString source, QList<QString> destinations)
{
    clear();

    this->SourceDrive = new DriveData(source);

    for (int i = 0; i < destinations.count(); i++) {
        DriveData* drive = new DriveData(destinations.at(i));
        this->DestinationDrives.append(drive);
    }
}

DriveData* CopyData::sourceDrive()
{
    return this->SourceDrive;
}

DriveData* CopyData::destinationDrive(int index)
{
    return this->DestinationDrives.at(index);
}

int CopyData::destinationCount() const
{
    return this->DestinationDrives.count();
}
