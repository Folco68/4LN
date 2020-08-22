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
