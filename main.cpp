#include "CopyData.hpp"
#include "MainWindow.hpp"
#include "ThreadAnalyze.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int ret = a.exec();

    ThreadAnalyze::release();
    CopyData::release();

    return ret;
}
