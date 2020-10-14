#ifndef DETECTION_H
#define DETECTION_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QCheckBox>
#include <QScrollBar>
#include <QToolBox>
#include <QListWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QAction>
#include <QFileDialog>
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include "opencv2/opencv.hpp"
#include "videoprocess.h"
#include "preprocess.h"

namespace Ui {
class Detection;
}

class Detection : public QWidget
{
    Q_OBJECT

public:
    explicit Detection(QWidget *parent = nullptr);
    ~Detection();
    //void mousePressEvent(QMouseEvent *event) override();
signals:
    void cvtGray(bool grayScale);
protected:
    void paintEvent(QPaintEvent *event) override;
public slots:
    void readImage();
    void readVideo();
    void showFrame(QPixmap frame);
    void grayScale();
    void getCurrentFrameInfo(int frameCount, int posFrame);
private:
    QImage cvtGrayScale(QImage image);
    Ui::Detection *ui;
    QLabel *labelOri;
    QCheckBox *videoGrayCheckBox;
    QScrollBar *labelOriScrollBar;
    int widthLabelOri = 1280;
    int heightLabelOri = 720;
    QLabel *labelROI;
    cv::Mat *image;
    VideoProcess *video = nullptr;
    PreProcess *preProcess = nullptr;
    bool isPlaying = false;
    bool convertToGrayScale = false;
    bool setted = false;
};

#endif // DETECTION_H
