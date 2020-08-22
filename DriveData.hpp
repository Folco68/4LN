#ifndef DRIVEDATA_HPP
#define DRIVEDATA_HPP

#include "FileData.hpp"
#include <QList>
#include <QString>

class DriveData
{
  public:
    explicit DriveData(QString basepath);
    QString basePath() const;
    int fileCount() const;
    FileData* file(int index);
    void addFile(FileData file);
    QList<FileData> filesMarkedForDeletion() const;
    QList<FileData> filesMarkedForCopy() const;

  private:
    QString BasePath;
    QList<FileData> FileList;
};

#endif // DRIVEDATA_HPP
