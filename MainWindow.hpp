/*
 * 4LN - Multiple Destinations Copier - batch copier to clone a content into several drives at once
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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "WidgetDestination.hpp"
#include "WidgetDrop.hpp"
#include "WidgetOverwrite.hpp"
#include "WindowAnalyze.hpp"
#include "WindowClone.hpp"
#include <QList>
#include <QMainWindow>
#include <QString>
#include <QTime>

enum { STEP_ZERO /* ;) */, STEP_DROP, STEP_SELECT_DEST, STEP_OVERWRITE, STEP_CLONE };

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

//
//  MainWindow
//
// This class describes the main interface
//
class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QString directory);
    ~MainWindow();

  private:
    Ui::MainWindow* ui;
    WidgetDrop* DropBox;
    WidgetDestination* DestinationBox;
    WidgetOverwrite* OverwriteBox;
    WindowAnalyze* WAnalyze;
    WindowClone* WClone;
    QString SourceDirectory;
    int MainWindowStep;
    QTime CloneTimer;

    void changeStep(int mws);
    void updateUI();

    // Slots
    void analyzeComplete();
    void deleteWindowAnalyze();
    void cloneComplete();
    void deleteWindowClone();
    void threadAborted();
};

#endif // MAINWINDOW_HPP
