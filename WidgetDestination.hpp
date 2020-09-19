#ifndef WIDGETDESTINATION_HPP
#define WIDGETDESTINATION_HPP

#include <QCommandLinkButton>
#include <QIcon>
#include <QList>
#include <QString>
#include <QWidget>

namespace Ui {
class WidgetDestination;
}

class WidgetDestination : public QWidget
{
    Q_OBJECT

  public:
    explicit WidgetDestination(QWidget *parent = nullptr);
    ~WidgetDestination();
    void refreshDestinations();
    void refreshDestinations(QString folder);
    QList<QString> destinationList() const;
    int selectedDestinations() const;

  private:
    Ui::WidgetDestination *ui;
    QList<QCommandLinkButton*> ButtonList;
    QIcon DestIcon;

  signals:
    void selectionChanged();
};

#define HZ_ITEM_COUNT 3

#endif // WIDGETDESTINATION_HPP
