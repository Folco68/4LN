#include "WindowDiff.hpp"
#include "Global.hpp"
#include "ui_WindowDiff.h"
#include <QPushButton>

WindowDiff::WindowDiff(QWidget *parent) :
      QDialog(parent),
      ui(new Ui::WindowDiff)
{
    ui->setupUi(this);
    setWindowTitle(WINDOW_TITLE);

    // Buttons
    connect(ui->ButtonClone, &QPushButton::clicked, [this]() { accept(); });
    connect(ui->ButtonCancel, &QPushButton::clicked, [this]() { reject(); });
}

WindowDiff::~WindowDiff()
{
    delete ui;
}
