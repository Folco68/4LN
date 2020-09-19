#ifndef WIDGETOVERWRITE_HPP
#define WIDGETOVERWRITE_HPP

#include <QWidget>

namespace Ui {
class WidgetOverwrite;
}

class WidgetOverwrite : public QWidget
{
    Q_OBJECT

  public:
    explicit WidgetOverwrite(QWidget *parent = nullptr);
    ~WidgetOverwrite();
    int value() const;

  private:
    Ui::WidgetOverwrite* ui;

    void spinboxValueChanged(int value);
    void sliderValueChanged(int value);
};

#endif // WIDGETOVERWRITE_HPP
