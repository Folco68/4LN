#ifndef SOURCEBOX_HPP
#define SOURCEBOX_HPP

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QGroupBox>
#include <QList>
#include <QUrl>

class SourceBox : public QGroupBox
{
    Q_OBJECT

  public:
    explicit SourceBox(QWidget* parent) : QGroupBox(parent) {} // Ctor is mMandatory because the UI factory creates the groupbox with a parent

  private:
    void dragEnterEvent(QDragEnterEvent* event) override; // Called when something is dragged over the object
    void dropEvent(QDropEvent* event) override;           // Called when something is dropped on the object

  signals:
    void directoryDropped(QString directory);
};

#endif // SOURCEBOX_HPP
