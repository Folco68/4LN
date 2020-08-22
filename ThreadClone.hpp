#ifndef THREADCLONE_HPP
#define THREADCLONE_HPP

#include <QObject>
#include <QString>
#include <QThread>

class ThreadClone : public QThread
{
    Q_OBJECT

  public:
    static ThreadClone* instance();
    static void release();
    void clone();

  private:
    static ThreadClone* threadclone;
    void run() override;

  signals:
    void copyingFile(QString filename);
    void copyingNextDrive(QString drivename);
    void cloneComplete();
};

#endif // THREADCLONE_HPP
