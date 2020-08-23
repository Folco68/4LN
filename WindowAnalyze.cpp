#include "WindowAnalyze.hpp"
#include "Global.hpp"
#include "ThreadAnalyze.hpp"
#include "ui_WindowAnalyze.h"
#include <QDir>
#include <QPushButton>

WindowAnalyze::WindowAnalyze(int progresscount) : QWidget(nullptr), ui(new Ui::WindowAnalyze)
{
    ui->setupUi(this);
    ui->ProgressBar->setMaximum(1 + progresscount * 2); // +1: source directory. *2: 1 for parsing + 1 for action to take
    setWindowTitle(WINDOW_TITLE);
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setMinimumWidth(PROGRESS_WINDOW_MIN_WIDTH);

    connect(ThreadAnalyze::instance(), &ThreadAnalyze::parsingFile, this, &WindowAnalyze::parsingFile, Qt::QueuedConnection);
    connect(ThreadAnalyze::instance(), &ThreadAnalyze::parsingNextDirectory, this, &WindowAnalyze::parsingNextDirectory, Qt::QueuedConnection);
    connect(ui->ButtonCancel, &QPushButton::clicked, ThreadAnalyze::instance(), &ThreadAnalyze::requestInterruption, Qt::QueuedConnection);

    adjustSize();
}

WindowAnalyze::~WindowAnalyze()
{
    delete ui;
}

void WindowAnalyze::parsingNextDirectory()
{
    int Progress = 1 + ui->ProgressBar->value();
    ui->ProgressBar->setValue(Progress);
}

void WindowAnalyze::parsingFile(QString filename)
{
    QString format = QDir::cleanPath(filename);
    ui->ProgressBar->setFormat(format);
}
