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
    // 单例模式
    static VideoThread* Get(){
        static VideoThread vt;
        return &vt;
    }

    // 线程入口函数
    void run();

    // 打开一号视频文件
    bool Open(const std::string file);
    // 打开二号视频文件
    bool Open2(const std::string file);
    //是否成功打开
    bool isOpen(){return ifOpen;}
    // 播放
    void Play();

    // 暂停
    void Pause();

    // 当前位置
    double GetPos();

    // 跳转视频
    /// @param frame int 帧位置
    bool Seek(int frame);
    bool Seek(double pos);

    // 开始导出视频
    bool StartSave(const std::string filename, int width = 0, int height = 0, bool isColor = true);

    // 停止保存视频，写入视频尾部帧索引
    void StopSave();

    // 设置水印
    void SetMark(cv::Mat mark);

private:
    // 线程同步锁
    QMutex mutex;
    // 1号视频源
    cv::VideoCapture cap1;
    // 2号视频源
    cv::VideoCapture cap2;

    cv::VideoWriter videoWriter;
    cv::Mat mark;
    // 文件是否打开
    bool ifOpen = false;
    // 程序是否已退出
    bool isExit = false;
    // 是否开始导出视频
    bool isWrite = false;
    // 是否播放
    bool isPlay = false;
    // 是否加水印
    bool isMark = false;
protected:
    // 单件模式防止创建对象，将构造函数置于保护下
    VideoThread();
    ~VideoThread();
signals:
    void ViewImageSrc(cv::Mat mat);
    void ViewDes(cv::Mat mat);
    void SaveEnd();
    void ViewSrc2(cv::Mat mat);
};

#endif // VIDEOTHREAD_H
