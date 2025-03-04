#include "videothread.h"

VideoThread::VideoThread() {
    start();
}

VideoThread::~VideoThread()
{
    mutex.lock();
    isExit = true;
    mutex.unlock();
}


bool VideoThread::Open(const std::string file)
{
    // Open是在其他线程中调用，调用时应仅单个线程可调用，加锁
    mutex.lock();
    bool bRes = cap1.open(file);
    // 解锁
    mutex.unlock();
    if(!bRes) return bRes;
    fps = cap1.get(cv::CAP_PROP_FPS);
    allFramesCount = cap1.get(cv::CAP_PROP_FRAME_COUNT);
    if(fps == 0) fps = 25;
    qDebug() << "file :" << QString::fromStdString(file) << " " << bRes;
    return bRes;
}

double VideoThread::GetPos()
{
    double pos = 0;
    mutex.lock();
    if(!cap1.isOpened()){
        mutex.unlock();
        return pos;
    }
    // 获取当前帧位置
    int curFrame = cap1.get(cv::CAP_PROP_POS_FRAMES);
    pos = (double)curFrame / allFramesCount;
    mutex.unlock();
    return pos;
}

bool VideoThread::Seek(int frame)
{
    if(!cap1.isOpened()) return false;
    mutex.lock();
    if(frame <= allFramesCount)
        cap1.set(cv::CAP_PROP_POS_FRAMES, frame);
    else{
        mutex.unlock();
        return false;
    }
    mutex.unlock();
    return true;
}

bool VideoThread::Seek(double pos)
{
    if(!cap1.isOpened()) return false;
    int frame = pos * allFramesCount;
    return Seek(frame);
}

void VideoThread::run()
{
    cv::Mat frame1;
    for(;;){
        mutex.lock();
        if(isExit){
            mutex.unlock();
            break;
        }
        if(!cap1.isOpened()){
            mutex.unlock();
            msleep(5);
            continue;
        }
        // read--读取一帧视频，解码并做颜色转换
        if(!cap1.read(frame1) || frame1.empty()){
            mutex.unlock();
            msleep(5);
            continue;
        }
        // 显示图像
        emit ViewImageSrc(frame1);
        msleep(1000 / fps);
        mutex.unlock();

    }
}
