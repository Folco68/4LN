#ifndef WIDGETDESTINATION_HPP
#define WIDGETDESTINATION_HPP

#include <QCheckBox>
#include <QStorageInfo>
#include <QString>
#include <QWidget>

namespace Ui {
class Destination;
}

class WidgetDestination : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetDestination(QStorageInfo storage);
    ~WidgetDestination();

    bool isSelected() const;
    QCheckBox* checkBox() const;
    QString drivePath() const;

private:
    Ui::Destination* ui;
    QString RootPath;
};

#endif // WIDGETDESTINATION_HPP
