#ifndef THREADANALYZE_HPP
#define THREADANALYZE_HPP

#include "CopyItem.hpp"
#include "ItemContainer.hpp"
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

    void analyze(QString source, QList<QString> destinations);

private:
    static ThreadAnalyze* threadanalyze;

    ItemContainer SourceItems;
    QList<ItemContainer> DestinationsItems;

    void run() override;
    void parseDirectory(ItemContainer* container, QString path = QString(""));

signals:
    void analyzeComplete();
    void parsingFile(QString filename);
    void parsingNextDirectory();
};

#endif // THREADANALYZE_HPP
