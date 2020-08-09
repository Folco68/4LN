#ifndef THREADANALYZE_HPP
#define THREADANALYZE_HPP

#include "DriveData.hpp"
#include <QObject>
#include <QThread>

class ThreadAnalyze : public QThread
{
    Q_OBJECT

  public:
    static ThreadAnalyze* instance();
    static void release();

    void analyze(QString source, QList<QString> destinations);

  private:
    static ThreadAnalyze* threadanalyze;

    void run() override;
    void parseDirectory(DriveData* drive, QString path = QString(""));

  signals:
    void analyzeComplete();
    void parsingFile(QString filename);
    void parsingNextDirectory();
};

#endif // THREADANALYZE_HPP
