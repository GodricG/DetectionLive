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
    int fps = video.get(cv::CAP_PROP_FPS);
    int frameBefore = 0;
    while (isPlaying)
    {
        if(video.isOpened())
        {
            QTime time;
            time.start();
            cv::Mat frame;
            //video.set(cv::CAP_PROP_POS_MSEC,53000);
            if(video.read(frame))
            {
                int currentFrame = video.get(cv::CAP_PROP_POS_FRAMES);
                int gap = abs(currentFrame-frameBefore);
                if(gap>=fps)
                {
                    emit currentFrameInfo(video.get(cv::CAP_PROP_FRAME_COUNT),video.get(cv::CAP_PROP_POS_FRAMES));
                    frameBefore = currentFrame;
                }
                if(toSet)
                {
                    video.set(cv::CAP_PROP_POS_FRAMES, posFrameToSet);
                    toSet = false;
                }
                float ratio = frame.cols*1.0 / frame.rows;
                cv::cuda::GpuMat d_frame;
                d_frame.upload(frame);
                if(ratio>1.0)
                {
                    cv::cuda::resize(d_frame,d_frame,cv::Size(width,(int)(width/ratio)));
                }
                else
                {
                    cv::cuda::resize(d_frame,d_frame,cv::Size((int)(height*ratio),height));
                }
                if(ifCvtGray)
                {
                    cv::cuda::cvtColor(d_frame,d_frame,CV_BGR2GRAY);
                    d_frame.download(frame);
                    QImage img(frame.data,frame.cols,frame.rows,frame.step,QImage::Format_Grayscale8);
                    emit newFrame(QPixmap::fromImage(img));
                }
                else
                {
                    d_frame.download(frame);
                    QImage img(frame.data,frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
                    emit newFrame(QPixmap::fromImage(img));
                }
                if(time.elapsed()<1000/fps)
                {
                    msleep(1000/fps - time.elapsed());
                }
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

void VideoProcess:: cvtGray(bool grayScale)
{
    ifCvtGray = grayScale;
}

void VideoProcess::setPosFrame(int posFrame)
{
    if(video.isOpened())
    {
        toSet = true;
        posFrameToSet = posFrame;
    }
    else
    {
        run();
    }
}
