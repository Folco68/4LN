#include "ItemContainer.hpp"

ItemContainer::ItemContainer(QString basepath) : BasePath(basepath)
{
}

void ItemContainer::init(QString path)
{
    this->BasePath = path;
}

void ItemContainer::clear()
{
    this->ItemList.clear();
}

QString ItemContainer::basePath() const
{
    return this->BasePath;
}

int ItemContainer::count() const
{
    return this->ItemList.count();
}

CopyItem* ItemContainer::getItem(int index)
{
    return &this->ItemList[index];
}

void ItemContainer::addItem(CopyItem item)
{
    this->ItemList.append(item);
}
