#include "widgetUI.h"
#include "ui_widgetUI.h"
#include <QFileDialog>
#include <string>
#include <QMessageBox>
#include "videothread.h"
#include "videowidget.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetUI)
{
    ui->setupUi(this);
    // 隐藏窗口边框
    setWindowFlags(Qt::FramelessWindowHint);

    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<cv::Mat>("cv::Mat&");
    connect(VideoThread::Get(), &VideoThread::ViewImageSrc, ui->src1Video, &VideoWidget::SetImage);

    startTimer(40);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::timerEvent(QTimerEvent *e)
{
    if(isSliderPress) return;
    double pos = VideoThread::Get()->GetPos();
    ui->playSlider->setValue(pos * 1000);
}

void Widget::on_openButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                "请选择视频文件",
                                                "",
                                                "Videos (*.mp4;*.avi;);");
    if(path.isEmpty()) return;
    // 中文路径支持
    std::string filePath = path.toLocal8Bit().data();

    if(!VideoThread::Get()->Open(filePath)){
        QMessageBox::information(this, "error", path + "打开失败！");
    }
}


void Widget::on_playSlider_sliderMoved(int position)
{
    //VideoThread::Get()->Seek((double)position / 1000 * VideoThread::Get()->allFramesCount);
    VideoThread::Get()->Seek((double)position / 1000);
}


void Widget::on_playSlider_sliderPressed()
{
    isSliderPress = true;
}


void Widget::on_playSlider_sliderReleased()
{
    isSliderPress = false;
}

