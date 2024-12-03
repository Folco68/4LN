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

#include "WidgetOverwrite.hpp"
#include "../Global.hpp"
#include "ui_WidgetOverwrite.h"
#include <QDoubleSpinBox>
#include <QSettings>
#include <QtGlobal>

WidgetOverwrite::WidgetOverwrite(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::WidgetOverwrite)
{
    ui->setupUi(this);

    // Connections between slider and spinbox
    connect(ui->SpinboxOverwrite, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](int value) { spinboxValueChanged(value); });
    connect(ui->SlideOverwrite, &QSlider::valueChanged, [this](int value) { sliderValueChanged(value); });

    // Restore previous setting
    QSettings Settings(ORGANIZATION_NAME, APPLICATION_NAME);
    ui->SlideOverwrite->setValue(Settings.value(KEY_OVERWRITE, DEFAULT_OVERWRITE).toInt());
}

WidgetOverwrite::~WidgetOverwrite()
{
    // Save overwrite setting
    QSettings Settings(ORGANIZATION_NAME, APPLICATION_NAME);
    Settings.setValue(KEY_OVERWRITE, ui->SlideOverwrite->value());
    delete ui;
}

void WidgetOverwrite::spinboxValueChanged(int value)
{
    ui->SlideOverwrite->blockSignals(true);
    ui->SlideOverwrite->setValue(value);
    ui->SlideOverwrite->blockSignals(false);
}

void WidgetOverwrite::sliderValueChanged(int value)
{
    ui->SpinboxOverwrite->blockSignals(true);
    ui->SpinboxOverwrite->setValue(value);
    ui->SpinboxOverwrite->blockSignals(false);
}

int WidgetOverwrite::value() const
{
    return ui->SpinboxOverwrite->value();
}
