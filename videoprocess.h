#ifndef VIDEOPROCESS_H
#define VIDEOPROCESS_H

#include <QThread>
#include <QImage>
#include <QPixmap>
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
public slots:

private:
    QString _videoPath;
    bool isPlaying = true;
    cv::VideoCapture video;
};

#endif // VIDEOPROCESS_H
