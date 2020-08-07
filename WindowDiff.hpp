#ifndef WINDOWDIFF_HPP
#define WINDOWDIFF_HPP

#include <QDialog>

namespace Ui {
class WindowDiff;
}

class WindowDiff : public QDialog
{
    Q_OBJECT

public:
    explicit WindowDiff(QWidget *parent = nullptr);
    ~WindowDiff();

private:
    Ui::WindowDiff *ui;
};

#endif // WINDOWDIFF_HPP
