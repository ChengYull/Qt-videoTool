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

    // 暂停按钮移动到播放按钮相同为止
    ui->pauseButton->setGeometry(ui->playButton->geometry());
    ui->pauseButton->hide(); // 隐藏暂停按钮

    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<cv::Mat>("cv::Mat&");
    connect(VideoThread::Get(), &VideoThread::ViewImageSrc, ui->src1Video, &VideoWidget::SetImage);
    connect(VideoThread::Get(), &VideoThread::ViewDes, ui->desVideo, &VideoWidget::SetImage);
    connect(VideoThread::Get(), &VideoThread::SaveEnd, this, &Widget::exportEnd);
    connect(VideoThread::Get(), &VideoThread::ViewSrc2, ui->src2Video, &VideoWidget::SetImage);

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
    VideoThread::Get()->Pause();
    isSliderPress = true;
}


void Widget::on_playSlider_sliderReleased()
{
    VideoThread::Get()->Play();
    isSliderPress = false;
}

// 设置过滤器
void Widget::on_setButton_clicked()
{
    // 初始化
    VideoFilter::Get()->Clear();
    isColor = true;
    isMerge = ui->mergeCheckBox->isChecked();

    // 图像裁剪
    double x = ui->xSpinBox->value();
    double y = ui->ySpinBox->value();
    double w = ui->wSpinBox->value();
    double h = ui->hSpinBox->value();
    if(x + y + w + h > 0.001){
        VideoFilter::Get()->Add(XTask{ XTASK_CLIP, {x, y, w, h} });
    }

    // 图像金字塔
    bool isPy = false;
    int down = ui->pyDownSpinBox->value();
    int up = ui->pyUpSpinBox->value();
    if(down > 0){
        isPy = true;
        VideoFilter::Get()->Add(XTask{ XTASK_PYDOWN, {(double)down} });
    }
    if(up > 0){
        isPy = true;
        VideoFilter::Get()->Add(XTask{ XTASK_PYUP, {(double)up} });
    }
    // 尺寸
    if(!isPy && ui->widthSpinBox->value() > 0 && ui->heightSpinBox->value() > 0){
        VideoFilter::Get()->Add(XTask{ XTASK_RESIZE,
                                      {(double)ui->widthSpinBox->value(),
                                       (double)ui->heightSpinBox->value()} });
    }
    // 对比度和亮度
    if(ui->bright_spinBox->value() > 0 ||
        ui->contrast_doubleSpinBox->value() > 1){
        VideoFilter::Get()->Add(XTask{ XTASK_GAIN,
                                      {(double)ui->bright_spinBox->value(),
                                       ui->contrast_doubleSpinBox->value()}
        });
    }
    // 旋转
    if(ui->rotateComboBox->currentIndex() != 0){
        VideoFilter::Get()->Add(XTask{ (XTaskType)(ui->rotateComboBox->currentIndex() + 1) });
    }
    // 镜像
    if(ui->flipComboBox->currentIndex() != 0){
        VideoFilter::Get()->Add(XTask{ (XTaskType)(ui->flipComboBox->currentIndex() + 4) });
    }
    // 灰度图
    if(ui->ColorComboBox->currentIndex() == 1){
        VideoFilter::Get()->Add(XTask{ XTASK_GRAY });
        isColor = false;
    }
    // 添加水印
    if(isMark){
        double x = ui->markXSpinBox->value();
        double y = ui->markYSpinBox->value();
        double a = ui->markADoubleSpinBox->value();
        VideoFilter::Get()->Add(XTask{ XTASK_MARK, { x, y, a } });
    }
    // 视频融合
    if(isMerge){
        double a = ui->mergeADoubleSpinBox->value();
        VideoFilter::Get()->Add(XTask{ XTASK_MERGE, {a} });
    }
    // 视频合并
    if(ui->sideCheckBox->isChecked()){
        VideoFilter::Get()->Add(XTask{ XTASK_SIDE});
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
    int width = VideoFilter::Get()->OutSize().width;
    int height = VideoFilter::Get()->OutSize().height;
    qDebug() << "width" << width;
    qDebug() << "height" << height;
    if(VideoThread::Get()->StartSave(filename, width, height, isColor)){
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

void Widget::on_playButton_clicked()
{
    if(!VideoThread::Get()->isOpen())
        return;
    ui->playButton->hide(); // 隐藏播放按钮
    ui->pauseButton->show(); // 显示暂停按钮
    VideoThread::Get()->Play();

}


void Widget::on_pauseButton_clicked()
{
    if(!VideoThread::Get()->isOpen())
        return;
    ui->pauseButton->hide(); // 隐藏暂停按钮
    ui->playButton->show(); // 显示播放按钮
    VideoThread::Get()->Pause();
}


void Widget::on_markButton_clicked()
{
    isMark = false;
    isMerge = false;
    QString path = QFileDialog::getOpenFileName(this,
                                                "请选择水印文件",
                                                "",
                                                "Videos (*.png;*.jpg;);");
    if(path.isEmpty()){
        return;
    }
    std::string file = path.toLocal8Bit().data();
    cv::Mat mark = cv::imread(file);
    if(mark.empty()) return;
    VideoThread::Get()->SetMark(mark);
    isMark = true;
}


void Widget::on_chooseSrc2Button_clicked()
{
    isMark = false;
    isMerge = false;
    QString path = QFileDialog::getOpenFileName(this,
                                                "请选择视频文件",
                                                "",
                                                "Videos (*.mp4;*.avi;);");
    if(path.isEmpty()) return;
    std::string file = path.toLocal8Bit().data();
    isMerge = VideoThread::Get()->Open2(file);
}

