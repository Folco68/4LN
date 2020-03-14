#ifndef DESTINATION_HPP
#define DESTINATION_HPP

#include <QStorageInfo>
#include <QWidget>

namespace Ui {
class Destination;
}

class Destination : public QWidget
{
    Q_OBJECT

public:
    explicit Destination(QStorageInfo storage);
    ~Destination();

private:
    Ui::Destination* ui;
};

#endif // DESTINATION_HPP
