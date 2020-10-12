#include "videoprocess.h"

VideoProcess::VideoProcess(QThread *parent):  QThread(parent)
{

}

VideoProcess::~VideoProcess()
{
    video.release();
}

void VideoProcess::setVideoPath(QString path)
{
    _videoPath = path;
}
void VideoProcess::run()
{
    video.open(_videoPath.toStdString());
    while (isPlaying)
    {
        if(video.isOpened())
        {
            cv::Mat frame;
            if(video.read(frame))
            {
                float ratio = frame.cols*1.0 / frame.rows;
                cv::cuda::GpuMat d_frame;
                d_frame.upload(frame);
                if(ratio>1.0)
                {
                    //cv::resize(frame,frame,cv::Size(1400,(int)(1400/ratio)));
                    cv::cuda::resize(d_frame,d_frame,cv::Size(1400,(int)(1400/ratio)));
                }
                else
                {
                    //cv::resize(frame,frame,cv::Size((int)(1050*ratio),1050));
                    cv::cuda::resize(d_frame,d_frame,cv::Size((int)(1050*ratio),1050));
                }
                d_frame.download(frame);
                QImage img(frame.data,frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
                emit newFrame(QPixmap::fromImage(img.rgbSwapped()));
            }
            else
            {

            }
        }
    }
    video.release();
}

void VideoProcess::startPlaying()
{
    isPlaying = true;
}

void VideoProcess::stopPlaying()
{
    isPlaying = false;
}
