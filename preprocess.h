#ifndef PREPROCESS_H
#define PREPROCESS_H
#include "opencv2/opencv.hpp"

class PreProcess
{
public:
    PreProcess();
    PreProcess(cv::Mat* input);
    void SetImage(cv::Mat *img);
    void SetROI(cv::Point anchor,int width,int height);
    void DetectEdge();
    cv::Mat GetROI();
private:
    cv::Mat *inputImg = nullptr;
    cv::Mat *outputImg = nullptr;
    cv::Mat ROI;
};

#endif // PREPROCESS_H
