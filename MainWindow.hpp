#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "Destination.hpp"
#include "WindowAnalyze.hpp"
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
    QList<Destination*> DestinationList;
    WindowAnalyze* Window;

    void updateUI();
    QList<Destination*> selectedDestinations() const;
    void deleteWindowAnalyze();

    // Buttons
    void refreshDestinations();
    void clone();
    void browseSource();

    // Slots
    void analyzeComplete();
    void analyzeCanceled();
};

#define HZ_DEST_MAX 5
//#define DEST_SPACER_SIZE 20

#endif // MAINWINDOW_HPP
