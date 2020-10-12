#ifndef VIDEOPROCESS_H
#define VIDEOPROCESS_H

#include <QThread>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QTime>
#include "opencv2/opencv.hpp"

class VideoProcess : public QThread
{
    Q_OBJECT
public:
    VideoProcess(QThread *parent = nullptr);
    VideoProcess(QString path,QThread *parent = nullptr);
    ~VideoProcess();
    void run() override;
    void setVideoPath(QString path);
    void startPlaying();
    void stopPlaying();
signals:
    void newFrame(QPixmap frame);
    void newFrameGray(QPixmap frame);
    void currentFrameInfo(int frameCount,int posFrame);
public slots:
    void cvtGray(bool grayScale);
    void setPosFrame(int posFrame);
private:
    QString _videoPath;
    bool isPlaying = true;
    bool ifCvtGray = false;
    bool toSet = false;
    cv::VideoCapture video;
    int width = 1280;
    int height = 720;
    int posFrameToSet = 0;
};

#endif // VIDEOPROCESS_H
