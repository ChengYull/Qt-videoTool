#include "videothread.h"
#include "videofilter.h"
VideoThread::VideoThread() {
    start();
}

VideoThread::~VideoThread()
{
    mutex.lock();
    isExit = true;
    mutex.unlock();
    wait();
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

bool VideoThread::StartSave(const std::string filename, int width, int height)
{
    qDebug() << "start save";
    Seek(0);
    mutex.lock();
    if(!cap1.isOpened()){
        qDebug() << "文件未打开";
        mutex.unlock();
        return false;
    }
    if(width <= 0 || height <= 0){
        width = cap1.get(cv::CAP_PROP_FRAME_WIDTH);
        height = cap1.get(cv::CAP_PROP_FRAME_HEIGHT);
    }
    videoWriter.open(filename,
                     cv::VideoWriter::fourcc('X', '2', '6', '4'),
                     this->fps,
                     cv::Size(width, height));
    if(!videoWriter.isOpened()){
        qDebug() << "开始导出失败" << QString::fromStdString(filename) << width << height;
        mutex.unlock();
        return false;
    }
    this->isWrite = true;
    mutex.unlock();
    return true;
}

void VideoThread::StopSave()
{
    qDebug() << "停止导出...";
    mutex.lock();
    videoWriter.release();
    isWrite = false;
    mutex.unlock();
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
            // 写入完成后自动停止导出
            if(isWrite){
                emit SaveEnd();
                StopSave();
            }
            msleep(5);
            continue;
        }
        // 显示图像1
        if(!isWrite)
            emit ViewImageSrc(frame1);
        // 过滤器
        cv::Mat des = VideoFilter::Get()->Filter(frame1, cv::Mat());
        // 显示输出图像
        if(!isWrite)
            emit ViewDes(des);
        if(isWrite){
            videoWriter.write(des);
        }
        if(!isWrite)
            msleep(1000 / fps);
        mutex.unlock();

    }
}
