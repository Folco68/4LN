#ifndef ITEMCONTAINER_HPP
#define ITEMCONTAINER_HPP

#include "CopyItem.hpp"
#include "Destination.hpp"
#include <QList>
#include <QString>

class ItemContainer
{
public:
    ItemContainer() {}
    explicit ItemContainer(QString basepath);
    void init(QString path);
    void clear();
    QString basePath() const;
    int count() const;
    CopyItem* getItem(int index);
    void addItem(CopyItem item);

private:
    QString BasePath;
    QList<CopyItem> ItemList;
};

#endif // ITEMCONTAINER_HPP
