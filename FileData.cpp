#include "FileData.hpp"

FileData::FileData(QString filename, qint64 size) : Filename(filename), Size(size), Process(DELETE_FILE)
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

void FileData::setProcess(FILE_PROCESS process)
{
    this->Process = process;
}

FILE_PROCESS FileData::process() const
{
    return this->Process;
}
