#ifndef COPYITEM_HPP
#define COPYITEM_HPP

#include <QString>

class CopyItem
{
public:
    CopyItem(QString filename, qint64 size);

private:
    QString Filename;
    qint64 Size;
};

#endif // COPYITEM_HPP
