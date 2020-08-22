#ifndef WINDOWCLONE_HPP
#define WINDOWCLONE_HPP

#include <QString>
#include <QWidget>

namespace Ui {
class WindowClone;
}

class WindowClone : public QWidget
{
    Q_OBJECT

  public:
    explicit WindowClone(int progresscount);
    ~WindowClone();

  private:
    Ui::WindowClone* ui;

    void copyingFile(QString filename);
    void copyingNextDrive(QString drivename);
};

#endif // WINDOWCLONE_HPP
