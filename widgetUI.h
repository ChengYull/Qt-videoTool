#ifndef WIDGETUI_H
#define WIDGETUI_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class WidgetUI;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void timerEvent(QTimerEvent *e) override;
private slots:
    void on_openButton_clicked();

    void on_playSlider_sliderMoved(int position);

    void on_playSlider_sliderPressed();

    void on_playSlider_sliderReleased();

private:
    Ui::WidgetUI *ui;
    bool isSliderPress = false;
};
#endif // WIDGETUI_H
