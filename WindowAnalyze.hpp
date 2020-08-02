#ifndef WINDOWANALYZE_HPP
#define WINDOWANALYZE_HPP

#include <QWidget>

namespace Ui {
class WindowAnalyze;
}

class WindowAnalyze : public QWidget
{
    Q_OBJECT

public:
    explicit WindowAnalyze(int progresscount);
    ~WindowAnalyze();
    void parsingNextDirectory();
    void parsingFile(QString filename);

private:
    Ui::WindowAnalyze *ui;
};

#endif // WINDOWANALYZE_HPP
