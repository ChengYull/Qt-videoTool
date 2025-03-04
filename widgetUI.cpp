#include "widgetUI.h"
#include "ui_widgetUI.h"
#include <QFileDialog>
#include <string>
#include <QMessageBox>
#include "videothread.h"
#include "videowidget.h"
#include "videofilter.h"
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
    connect(VideoThread::Get(), &VideoThread::ViewDes, ui->desVideo, &VideoWidget::SetImage);
    connect(VideoThread::Get(), &VideoThread::SaveEnd, this, &Widget::exportEnd);

    // 开启定时器刷新图像
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

// 设置过滤器
void Widget::on_setButton_clicked()
{
    VideoFilter::Get()->Clear();
    // 对比度和亮度
    if(ui->bright_spinBox->value() > 0 ||
        ui->contrast_doubleSpinBox->value() > 1){
        VideoFilter::Get()->Add(XTask{ XTASK_GAIN,
                                      {(double)ui->bright_spinBox->value(),
                                       ui->contrast_doubleSpinBox->value()}
        });
    }

}

// 导出视频
void Widget::on_exportButton_clicked()
{

    if(isExport){
        // 停止导出
        VideoThread::Get()->StopSave();
        isExport = false;
        ui->exportButton->setText("导出");
        QMessageBox::information(this, "导出成功", "成功导出到本地!");
        return;
    }
    QString path = QFileDialog::getSaveFileName(this,
                                                "请选择视频文件位置",
                                                "out1.avi",
                                                "Videos (*.mp4;*.avi;);");
    if(path.isEmpty()) return;
    std::string filename = path.toLocal8Bit().data();
    if(VideoThread::Get()->StartSave(filename)){
        isExport = true;
        ui->exportButton->setText("停止导出");
    }
}

void Widget::exportEnd()
{
    isExport = false;
    ui->exportButton->setText("导出");
    QMessageBox::information(this, "导出成功", "成功导出到本地!");
}

