/*
 * MDC - Multiple Destinations Copier - batch copier to clone a content into several drives at once
 * Copyright (C) 2020 Martial Demolins AKA Folco
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * mail: martial <dot> demolins <at> gmail <dot> com
 */

#ifndef THREADANALYZE_HPP
#define THREADANALYZE_HPP

#include "DriveData.hpp"
#include <QObject>
#include <QThread>

//
//  ThreadAnalyze
//
// This class describes the thread which compares source and destinations, to set the corresponding process for each file
//
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
