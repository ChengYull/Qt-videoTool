#ifndef IMAGEPRO_H
#define IMAGEPRO_H
#include <opencv2/opencv.hpp>
class ImagePro
{
public:
    // 设置原图, 会清理处理结果
    void Set(cv::Mat mat1, cv::Mat mat2);
    // 获取处理后的结果
    cv::Mat Get();
    // 设置亮度和对比度
    /// @param bright double 亮度 0~100
    /// @param contrast double 对比度 1.0~3.0
    void Gain(double bright, double contrast);

    // 旋转
    void Rotate90();
    void Rotate180();
    void Rotate270();

    // 镜像
    void FlipX();
    void FlipY();
    void FlipXY();

    // 图像尺寸
    void Resize(int width, int height);

    // 获取输出尺寸
    cv::Size OutSize();

    // 图片金字塔
    void PyDown(int count);
    void PyUp(int count);

    // 画面裁剪
    void Clip(int x, int y, int w, int h);

    // 灰度图
    void Gray();

    // 水印
    void Mark(int x, int y, double alpha);

    // 融合
    void Merge(double alpha);
    ImagePro();

private:
    // 原图
    cv::Mat src1, src2;

    // 目标图
    cv::Mat des;

};

#endif // IMAGEPRO_H
