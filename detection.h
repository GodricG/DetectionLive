#ifndef DETECTION_H
#define DETECTION_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
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
protected:
    void paintEvent(QPaintEvent *event) override;
public slots:
    void readImage();
    void readVideo();
    void showFrame(QPixmap frame);
private:
    void TreeWidgetAddItem(QTreeWidget *widget,QTreeWidgetItem *treeItem, QString headerString,QPushButton *pushButton, QString iconPath);
    void TreeWidgetAddPushButton(QTreeWidget *widget, QTreeWidgetItem *treeItem, QString buttonText);
    Ui::Detection *ui;
    QLabel *labelOri;
    cv::Mat *image;
    VideoProcess *video;
};

#endif // DETECTION_H
