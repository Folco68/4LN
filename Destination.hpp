#ifndef DESTINATION_HPP
#define DESTINATION_HPP

#include <QCheckBox>
#include <QStorageInfo>
#include <QString>
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
    QCheckBox* checkBox() const;
    bool isSelected() const;
    QString drivePath() const;

private:
    Ui::Destination* ui;
    QString RootPath;
};

#endif // DESTINATION_HPP
