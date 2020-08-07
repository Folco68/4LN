#include "CopyItem.hpp"

CopyItem::CopyItem(QString path, QString filename, qint64 size) : Path(path), Filename(filename), Size(size), Process(DELETE_FILE)
{
}

QString CopyItem::path() const
{
    return this->Path;
}

QString CopyItem::filename() const
{
    return this->Filename;
}

qint64 CopyItem::size() const
{
    return this->Size;
}

void CopyItem::setProcess(ITEM_PROCESS process)
{
    this->Process = process;
}

ITEM_PROCESS CopyItem::getProcess() const
{
    return this->Process;
}
