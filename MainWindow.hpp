#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "Destination.hpp"
#include <QGridLayout>
#include <QList>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    QGridLayout* DestinationGrid;
    QList<Destination*> DestinationList;

    void refreshDeviceList();
    void displayDeviceStatus();
    void clone() const;
    void browseSource();
    void updateUI();
};

#define HZ_DEVICE_COUNT 5
#define DEST_SPACER_SIZE 20

#endif // MAINWINDOW_HPP
