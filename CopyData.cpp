#include "CopyData.hpp"

CopyData* CopyData::copydata = nullptr;

CopyData* CopyData::instance()
{
    if (copydata == nullptr) {
        copydata = new CopyData;
    }
    return copydata;
}

void CopyData::release()
{
    delete copydata;
}

CopyData::~CopyData()
{
    clear();
    copydata = nullptr;
}

void CopyData::clear()
{
    // Delete source
    delete this->SourceDrive;
    this->SourceDrive = nullptr;

    // Delete destinations
    while (!this->DestinationDrives.isEmpty()) {
        delete this->DestinationDrives.takeLast();
    }
}

void CopyData::init(QString source, QList<QString> destinations)
{
    clear();

    this->SourceDrive = new DriveData(source);

    for (int i = 0; i < destinations.count(); i++) {
        DriveData* drive = new DriveData(destinations.at(i));
        this->DestinationDrives.append(drive);
    }
}

DriveData* CopyData::sourceDrive()
{
    return this->SourceDrive;
}

DriveData* CopyData::destinationDrive(int index)
{
    return this->DestinationDrives.at(index);
}

int CopyData::destinationCount() const
{
    return this->DestinationDrives.count();
}
