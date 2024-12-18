/*
 * 4LN - Multiple Destinations Copier - batch copier to clone a content into several drives at once
 * Copyright (C) 2020-2025 Martial Demolins AKA Folco
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

#ifndef FILEDATA_HPP
#define FILEDATA_HPP

#include <QString>

// Operation performed on the file/directory
typedef enum {
    DELETE_FILE,
    COPY_FILE,
    IGNORE_FILE
} FILE_PROCESS;

// File type (regular file or directory)
typedef enum {
    TYPE_DIRECTORY,
    TYPE_FILE
} FILE_TYPE;
#define NO_SIZE 0

//
//  FileData
//
// This class describes an entry (file or directory) in the source or in a destination
//
class FileData
{
  public:
    FileData(QString filename, qint64 size, FILE_PROCESS process, FILE_TYPE type);

    QString      filename() const;
    qint64       size() const;
    FILE_PROCESS process() const;
    FILE_TYPE    type() const;
    void         setProcess(FILE_PROCESS process);

  private:
    QString      Filename;
    qint64       Size;
    FILE_PROCESS Process;
    FILE_TYPE    Type;
};

#endif // FILEDATA_HPP
