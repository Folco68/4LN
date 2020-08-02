#ifndef THREADANALYZE_HPP
#define THREADANALYZE_HPP

#include "CopyItem.hpp"
#include "Destination.hpp"
#include <QList>
#include <QMutex>
#include <QObject>
#include <QThread>

class ThreadAnalyze : public QThread
{
    Q_OBJECT

public:
    static ThreadAnalyze* instance();
    static void release();

    void analyze(QString source, QList<Destination*> destinations);

private:
    static ThreadAnalyze* threadanalyze;

    QString SourcePath;
    QList<Destination*> DestinationList;

    QList<CopyItem> SourceItems;
    QList<QList<CopyItem>> DestinationItems;

    void run() override;
    void parseDirectory(QString directory, QList<CopyItem>* list);

signals:
    void analyzeComplete();
    void analyzeCanceled();
    void parsingFile(QString filename);
    void parsingNextDirectory();
};

#endif // THREADANALYZE_HPP
