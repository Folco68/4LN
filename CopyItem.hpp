#ifndef COPYITEM_HPP
#define COPYITEM_HPP

#include <QString>

typedef enum { DELETE_FILE, COPY_FILE, IGNORE_FILE } ITEM_PROCESS;

class CopyItem
{
public:
    CopyItem(QString path, QString filename, qint64 size);
    QString path() const;
    QString filename() const;
    qint64 size() const;
    void setProcess(ITEM_PROCESS process);
    ITEM_PROCESS getProcess() const;

private:
    QString Path;
    QString Filename;
    qint64 Size;
    ITEM_PROCESS Process;
};

#endif // COPYITEM_HPP
