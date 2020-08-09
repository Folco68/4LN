#include "FileData.hpp"

FileData::FileData(QString path, QString filename, qint64 size) : Path(path), Filename(filename), Size(size), Process(DELETE_FILE)
{
}

QString FileData::path() const
{
    return this->Path;
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
