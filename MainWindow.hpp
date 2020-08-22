#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "WidgetDestination.hpp"
#include "WindowAnalyze.hpp"
#include "WindowClone.hpp"
#include <QGridLayout>
#include <QList>
#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    QGridLayout* DestinationGrid;
    QList<WidgetDestination*> DestinationList;
    WindowAnalyze* WAnalyze;
    WindowClone* WClone;

    void updateUI();
    QList<QString> selectedDrives() const;
    int selectedDrivesCount() const;

    // Buttons
    void refreshDestinations();
    void clone();
    void browseSource();

    // Slots
    void analyzeComplete();
    void deleteWindowAnalyze();
    void cloneComplete();
    void deleteWindowClone();
};

#define HZ_DEST_MAX 5
//#define DEST_SPACER_SIZE 20

#endif // MAINWINDOW_HPP
