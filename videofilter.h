#ifndef VIDEOFILTER_H
#define VIDEOFILTER_H
#include <opencv2/opencv.hpp>
#include <vector>
enum XTaskType{
    XTASK_NONE,
    XTASK_GAIN, // 亮度对比度调整
    XTASK_ROTATE_90,
    XTASK_ROTATE_180,
    XTASK_ROTATE_270,
    XTASK_FLIPX,
    XTASK_FLIPY,
    XTASK_FLIPXY,
    XTASK_RESIZE,
    XTASK_PYDOWN,
    XTASK_PYUP,
    XTASK_CLIP,
    XTASK_GRAY,
    XTASK_MARK,
    XTASK_MERGE

};

struct XTask{
    XTaskType type;
    std::vector<double> para;
};

class VideoFilter
{
public:
    virtual cv::Mat Filter(cv::Mat mat1, cv::Mat mat2) = 0;
    virtual void Add(XTask task) = 0;
    virtual void Clear() = 0;
    virtual cv::Size OutSize() = 0;
    static VideoFilter *Get();
    virtual ~VideoFilter();
protected:
    VideoFilter();
    cv::Size outSize;
};

#endif // VIDEOFILTER_H
