#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QThread>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QMutex>
class VideoThread : public QThread
{

    Q_OBJECT

public:
    int fps = 0;
    int allFramesCount = 0;
    // 单件模式
    static VideoThread* Get(){
        static VideoThread vt;
        return &vt;
    }

    // 线程入口函数
    void run();
    // 打开一号视频文件
    bool Open(const std::string file);
    // 当前位置
    double GetPos();
    // 跳转视频
    // @param frame int 帧位置
    bool Seek(int frame);
    bool Seek(double pos);
private:
    // 线程同步锁
    QMutex mutex;
    // 1号视频源
    cv::VideoCapture cap1;
    bool isExit = false;
protected:
    // 单件模式防止创建对象，将构造函数置于保护下
    VideoThread();
    ~VideoThread();
signals:
    void ViewImageSrc(cv::Mat mat);
};

#endif // VIDEOTHREAD_H
