#include "videofilter.h"
#include <QMutex>
#include "imagepro.h"
class CXFilter : public VideoFilter{
public:
    std::vector<XTask> tasks;
    // 保证线程安全
    QMutex mutex;

    cv::Mat Filter(cv::Mat mat1, cv::Mat mat2){
        mutex.lock();
        ImagePro p;
        p.Set(mat1, mat2);
        for(XTask task : tasks){
            switch(task.type){
            case XTASK_GAIN:
                // 亮度对比度调整
                p.Gain(task.para[0], task.para[1]);
                break;
            default:
                break;
            }
        }
        cv::Mat re = p.Get();
        mutex.unlock();
        return re;
    }

    void Add(XTask task){
        mutex.lock();
        tasks.push_back(task);
        mutex.unlock();
    }
    void Clear(){
        mutex.lock();
        tasks.clear();
        mutex.unlock();
    }

};

VideoFilter *VideoFilter::Get()
{
    static CXFilter cx;
    return &cx;
}

VideoFilter::~VideoFilter()
{

}

VideoFilter::VideoFilter() {}
