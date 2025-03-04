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

    void on_setButton_clicked();

    void on_exportButton_clicked();

    void exportEnd();
private:
    Ui::WidgetUI *ui;
    bool isSliderPress = false;
    bool isExport = false;
};
#endif // WIDGETUI_H
