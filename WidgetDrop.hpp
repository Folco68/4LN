#ifndef WIDGETDROP_HPP
#define WIDGETDROP_HPP

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QString>
#include <QWidget>

namespace Ui {
class WidgetDrop;
}

class WidgetDrop : public QWidget
{
    Q_OBJECT

  public:
    explicit WidgetDrop(QWidget* parent = nullptr);
    ~WidgetDrop() override;

  private:
    Ui::WidgetDrop* ui;
    void dragEnterEvent(QDragEnterEvent* event) override; // Called when something is dragged over the object
    void dropEvent(QDropEvent* event) override;           // Called when something is dropped on the object

  signals:
    void directoryDropped(QString directory);
};

#endif // WIDGETDROP_HPP
