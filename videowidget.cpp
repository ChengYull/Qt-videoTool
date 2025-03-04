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
    if(img.isNull()){
        uchar *buf = new uchar[width() * height() * 3];
        img = QImage(buf, width(), height(), QImage::Format_RGB888);
    }
    cv::Mat des;
    cv::resize(mat, des, cv::Size(img.width(), img.height()));
    cv::cvtColor(des, des, cv::COLOR_RGB2BGR);
    // mat转QImage
    memcpy(img.bits(), des.data, des.cols * des.rows * des.elemSize());
    //memcpy(img.bits(), mat.data, mat.cols * mat.rows * mat.elemSize());
    update();
}
