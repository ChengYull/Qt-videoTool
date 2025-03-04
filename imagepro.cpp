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



ImagePro::ImagePro() {}
