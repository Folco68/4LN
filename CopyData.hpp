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

#ifndef COPYDATA_HPP
#define COPYDATA_HPP

#include "DriveData.hpp"
#include <QList>
#include <QString>

//
//  CopyData
//
// This class contains all the data used to perform cloning:
// - input data from UI
// - data resulting from the analyze thread and used to execute cloning
//
class CopyData
{
  public:
    static CopyData* instance();
    static void release();

    void init(QString source, QList<QString> destinations);
    int destinationCount() const;
    int filesCount() const;
    DriveData* sourceDrive();
    DriveData* destinationDrive(int index);

  private:
    static CopyData* copydata;
    ~CopyData();
    void clear();

    DriveData* SourceDrive = nullptr;
    QList<DriveData*> DestinationDrives;
};

#endif // COPYDATA_HPP
