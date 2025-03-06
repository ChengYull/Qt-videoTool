#include "imagepro.h"

// 设置原图, 会清理处理结果
void ImagePro::Set(cv::Mat mat1, cv::Mat mat2){
    if(mat1.empty()) return;
    this->src1 = mat1;
    this->src2 = mat2;
    this->src1.copyTo(des);
}

cv::Mat ImagePro::Get()
{
    return des;
}

// 设置亮度和对比度
/// @param bright double 亮度 0~100
/// @param contrast double 对比度 1.0~3.0
void ImagePro::Gain(double bright, double contrast){
    if(des.empty()) return;
    // 亮度对比度
    des.convertTo(des, -1, contrast, bright);
}

void ImagePro::Rotate90()
{
    if(des.empty()) return;
    cv::rotate(des, des, cv::ROTATE_90_CLOCKWISE);
}
void ImagePro::Rotate180()
{
    if(des.empty()) return;
    cv::rotate(des, des, cv::ROTATE_180);
}
void ImagePro::Rotate270()
{
    if(des.empty()) return;
    cv::rotate(des, des, cv::ROTATE_90_COUNTERCLOCKWISE);
}
// 水平翻转
void ImagePro::FlipX()
{
    if(des.empty()) return;
    cv::flip(des, des, 1);
}
// 垂直翻转
void ImagePro::FlipY()
{
    if(des.empty()) return;
    cv::flip(des, des, 0);
}
void ImagePro::FlipXY()
{
    if(des.empty()) return;
    cv::flip(des, des, -1);
}

void ImagePro::Resize(int width, int height)
{
    if(des.empty()) return;
    cv::resize(des, des, cv::Size(width, height));
}

cv::Size ImagePro::OutSize()
{
    if(des.empty()) return cv::Size();
    return cv::Size(des.cols, des.rows);
}

void ImagePro::PyDown(int count)
{
    if(des.empty()) return;
    for(int i = 0; i < count; ++i)
        cv::pyrDown(des, des);
}
void ImagePro::PyUp(int count)
{
    if(des.empty()) return;
    for(int i = 0; i < count; ++i)
        cv::pyrUp(des, des);
}

void ImagePro::Clip(int x, int y, int w, int h)
{
    if(des.empty()) return;
    if(x < 0 || y < 0 || y <= 0 || h <= 0) return;
    if(x > des.cols || y > des.rows) return;
    des = des(cv::Rect(x, y, w, h));
}

void ImagePro::Gray()
{
    if(des.empty()) return;
    cv::cvtColor(des, des, cv::COLOR_BGR2GRAY);
}

void ImagePro::Mark(int x, int y, double alpha)
{
    if(src2.empty() || des.empty()) return;
    cv::Mat ROI = des(cv::Rect(x, y, src2.cols, src2.rows));
    cv::addWeighted(src2, alpha, ROI, 1 - alpha, 0, ROI);
}

void ImagePro::Merge(double alpha)
{
    if(src2.empty() || des.empty()) return;
    // 大小改为一致再融合
    cv::resize(src2, src2, des.size());
    cv::Mat tmpMat = src2.clone();
    if(src2.type() == CV_8UC1)
        cv::cvtColor(src2, tmpMat, cv::COLOR_GRAY2BGR);
    cv::addWeighted(tmpMat, alpha, des, 1 - alpha, 0, des);
}

void ImagePro::Side()
{
    if(src2.empty() || des.empty()) return;
    // 保持高度一致
    cv::resize(src2, src2, cv::Size(src2.size().width, des.size().height));
    int dw = des.cols + src2.cols;
    int dh = des.rows;
    // 拷贝原输出
    cv::Mat tmpMat = des.clone();
    // 重新分配空间
    des = cv::Mat(cv::Size(dw, dh), src1.type());
    // 分别定义两块ROI区域
    cv::Mat r1 = des(cv::Rect(0, 0, tmpMat.cols, dh));
    cv::Mat r2 = des(cv::Rect(tmpMat.cols, 0, src2.cols, dh));
    // 填充ROI区域
    tmpMat.copyTo(r1);
    src2.copyTo(r2);
}
ImagePro::ImagePro() {}
