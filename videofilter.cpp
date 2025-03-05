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
            case XTASK_ROTATE_90:
                p.Rotate90();
                break;
            case XTASK_ROTATE_180:
                p.Rotate180();
                break;
            case XTASK_ROTATE_270:
                p.Rotate270();
                break;
            case XTASK_FLIPX:
                p.FlipX();
                break;
            case XTASK_FLIPY:
                p.FlipY();
                break;
            case XTASK_FLIPXY:
                p.FlipXY();
                break;
            case XTASK_RESIZE:
                p.Resize(task.para[0], task.para[1]);
                break;
            case XTASK_PYDOWN:
                p.PyDown(task.para[0]);
                break;
            case XTASK_PYUP:
                p.PyUp(task.para[0]);
                break;
            case XTASK_CLIP:
                p.Clip(task.para[0], task.para[1], task.para[2], task.para[3]);
                break;
            case XTASK_GRAY:
                p.Gray();
                break;
            case XTASK_MARK:
                p.Mark(task.para[0], task.para[1], task.para[2]);
                break;
            case XTASK_MERGE:
                p.Merge(task.para[0]);
                break;
            default:
                break;
            }
        }
        cv::Mat re = p.Get();
        outSize = re.size();
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

    cv::Size OutSize(){
        return outSize;
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
