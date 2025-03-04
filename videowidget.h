#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QOpenGLWidget>
#include <opencv2/opencv.hpp>
class VideoWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    VideoWidget(QWidget *p);
    void paintEvent(QPaintEvent *e);

public slots:
    void SetImage(cv::Mat mat);

protected:
    QImage img;
};

#endif // VIDEOWIDGET_H
