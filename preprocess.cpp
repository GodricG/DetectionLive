#include "preprocess.h"
PreProcess::PreProcess()
{
    inputImg = new cv::Mat(300,300,CV_8UC1,cv::Scalar(128));
}

PreProcess::PreProcess(cv::Mat* input)
{
    inputImg = input;
}

void PreProcess::SetImage(cv::Mat *img)
{
    inputImg = img;
}

void PreProcess::SetROI(cv::Point anchor, int width, int height)
{
    cv::Rect rect(anchor.x,anchor.y,width,height);
    ROI = (*inputImg)(rect);
}

void PreProcess::DetectEdge()
{
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    cv::Sobel(ROI,grad_x,CV_8UC1,1,0);
    cv::Sobel(ROI,grad_y,CV_8UC1,0,1);
    cv::convertScaleAbs(grad_x,abs_grad_x);
    cv::convertScaleAbs(grad_y,abs_grad_y);
    cv::addWeighted(abs_grad_x,0.5,abs_grad_y,0.5,0,ROI);
}

cv::Mat PreProcess::GetROI()
{
    return ROI;
}

