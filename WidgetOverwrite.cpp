#include "WidgetOverwrite.hpp"
#include "Global.hpp"
#include "ui_WidgetOverwrite.h"
#include <QDoubleSpinBox>
#include <QSettings>
#include <QtGlobal>

WidgetOverwrite::WidgetOverwrite(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::WidgetOverwrite)
{
    ui->setupUi(this);

    // Connections between slider and spinbox
    connect(ui->SpinboxOverwrite, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](int value) { spinboxValueChanged(value); });
    connect(ui->SlideOverwrite, &QSlider::valueChanged, [this](int value) { sliderValueChanged(value); });

    // Restore previous setting
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    ui->SlideOverwrite->setValue(settings.value(KEY_OVERWRITE, DEFAULT_OVERWRITE).toInt());
}

WidgetOverwrite::~WidgetOverwrite()
{
    // Save overwrite setting
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.setValue(KEY_OVERWRITE, ui->SlideOverwrite->value());
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
