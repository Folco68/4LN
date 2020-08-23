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
