#ifndef VIDEOFILTER_H
#define VIDEOFILTER_H
#include <opencv2/opencv.hpp>
#include <vector>
enum XTaskType{
    XTASK_NONE,
    XTASK_GAIN, // 亮度对比度调整

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
    static VideoFilter *Get();
    virtual ~VideoFilter();
protected:
    VideoFilter();

};

#endif // VIDEOFILTER_H
