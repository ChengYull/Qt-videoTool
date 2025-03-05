#include "videowidget.h"
#include <QPainter>
VideoWidget::VideoWidget(QWidget *p) : QOpenGLWidget(p) {

}

void VideoWidget::paintEvent(QPaintEvent *e){
    QPainter p;
    p.begin(this);
    p.drawImage(QPoint(0, 0), img);
    p.end();
}

void VideoWidget::SetImage(cv::Mat mat)
{
    QImage::Format fmt = QImage::Format_RGB888;
    int pixSize = 3;
    if(mat.type() == CV_8UC1){
        fmt = QImage::Format_Grayscale8;
        pixSize = 1;
    }
    if(img.isNull() || img.format() != fmt){
        delete img.bits();
        uchar *buf = new uchar[width() * height() * pixSize];
        img = QImage(buf, width(), height(), fmt);
    }
    cv::Mat des;
    cv::resize(mat, des, cv::Size(img.width(), img.height()));
    if(pixSize > 1)
        cv::cvtColor(des, des, cv::COLOR_RGB2BGR);
    // mat转QImage
    memcpy(img.bits(), des.data, des.cols * des.rows * des.elemSize());
    //memcpy(img.bits(), mat.data, mat.cols * mat.rows * mat.elemSize());
    update();
}
