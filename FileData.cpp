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

#include "FileData.hpp"

FileData::FileData(QString filename, qint64 size, FILE_PROCESS process, FILE_TYPE type) : Filename(filename), Size(size), Process(process), Type(type)
{
}

QString FileData::filename() const
{
    return this->Filename;
}

qint64 FileData::size() const
{
    return this->Size;
}

FILE_PROCESS FileData::process() const
{
    return this->Process;
}

FILE_TYPE FileData::type() const
{
    return this->Type;
}

void FileData::setProcess(FILE_PROCESS process)
{
    this->Process = process;
}
