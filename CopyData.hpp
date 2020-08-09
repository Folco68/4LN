#ifndef COPYDATA_HPP
#define COPYDATA_HPP

#include "DriveData.hpp"
#include <QList>
#include <QString>

class CopyData
{
  public:
    static CopyData* instance();
    static void release();
    void init(QString source, QList<QString> destinations);
    DriveData* sourceDrive();
    DriveData* destinationDrive(int index);
    int destinationCount() const;

  private:
    static CopyData* copydata;
    ~CopyData();
    void clear();

    DriveData* SourceDrive = nullptr;
    QList<DriveData*> DestinationDrives;
};

#endif // COPYDATA_HPP
