/*
 * 4LN - Multiple Destinations Copier - batch copier to clone a content into several drives at once
 * Copyright (C) 2020-2025 Martial Demolins AKA Folco
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

#ifndef WIDGETOVERWRITE_HPP
#define WIDGETOVERWRITE_HPP

#include <QWidget>

namespace Ui {
    class WidgetOverwrite;
}

class WidgetOverwrite: public QWidget
{
    Q_OBJECT

  public:
    explicit WidgetOverwrite(QWidget* parent = nullptr);
    ~WidgetOverwrite();
    int value() const;

  private:
    Ui::WidgetOverwrite* ui;

    void spinboxValueChanged(int value);
    void sliderValueChanged(int value);
};

#endif // WIDGETOVERWRITE_HPP