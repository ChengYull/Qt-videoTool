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
    /// @param frame int 帧位置
    bool Seek(int frame);
    bool Seek(double pos);

    // 开始导出视频
    bool StartSave(const std::string filename, int width = 0, int height = 0);

    // 停止保存视频，写入视频尾部帧索引
    void StopSave();

private:
    // 线程同步锁
    QMutex mutex;
    // 1号视频源
    cv::VideoCapture cap1;
    cv::VideoWriter videoWriter;

    bool isExit = false;
    // 是否开始导出视频
    bool isWrite = false;
protected:
    // 单件模式防止创建对象，将构造函数置于保护下
    VideoThread();
    ~VideoThread();
signals:
    void ViewImageSrc(cv::Mat mat);
    void ViewDes(cv::Mat mat);
    void SaveEnd();
};

#endif // VIDEOTHREAD_H
