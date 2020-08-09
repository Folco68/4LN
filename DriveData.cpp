#include "DriveData.hpp"

DriveData::DriveData(QString basepath) : BasePath(basepath)
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
