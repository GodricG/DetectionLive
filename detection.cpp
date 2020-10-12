#include "detection.h"
#include "ui_detection.h"

Detection::Detection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Detection)
{
    ui->setupUi(this);
    image = new cv::Mat;
    this->layout()->setMargin(0);

    QToolBox *toolBox = new QToolBox;
    toolBox->setMaximumWidth(200);
    //放置toolBox的容器
    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(toolBox);
    widgetLayout->setMargin(0);
    ui->widget->setLayout(widgetLayout);
    //放置图片主窗口的容器
    QHBoxLayout *hLayout = new QHBoxLayout;
    labelOri = new QLabel(this);
    labelOri->setMaximumSize(1400,1050);
    labelOri->setMinimumSize(600,450);
    hLayout->addWidget(labelOri);
    //放置三个副窗口的容器
    QVBoxLayout *labelsVLayout = new QVBoxLayout;
    QLabel *labelROI = new QLabel(this);
    labelROI->setMaximumWidth(200);
    labelROI->setMinimumWidth(200);
    labelsVLayout->addWidget(labelROI);
    QLabel *labelHistogram = new QLabel(this);
    labelHistogram->setMaximumWidth(200);
    labelHistogram->setMinimumWidth(200);
    labelsVLayout->addWidget(labelHistogram);
    QLabel *labelDetecion = new QLabel(this);
    labelDetecion->setMaximumWidth(200);
    labelDetecion->setMinimumWidth(200);
    labelsVLayout->addWidget(labelDetecion);
    //将上述两个容器水平放置
    QHBoxLayout *hMainLayout = new QHBoxLayout;
    hMainLayout->addLayout(hLayout);
    hMainLayout->addLayout(labelsVLayout);
    //将水平容器放入groupBox
    ui->groupBox->setLayout(hMainLayout);

    QPushButton *loadImage = new QPushButton(QStringLiteral("读取图片"),this);
    QPushButton *loadVideo = new QPushButton(QStringLiteral("读取视频"),this);
    QVBoxLayout *loadLayout = new QVBoxLayout;
    loadLayout->setMargin(0);
    loadLayout->addWidget(loadImage);
    loadLayout->addWidget(loadVideo);
    loadLayout->setAlignment(Qt::AlignTop);
    QWidget *loadWidget = new QWidget;
    loadWidget->setLayout(loadLayout);
    toolBox->insertItem(0,loadWidget,QStringLiteral("加载"));

    QPushButton *grayScale = new QPushButton(QStringLiteral("灰度化"),this);
    QVBoxLayout *procLayout = new QVBoxLayout;
    procLayout->setMargin(0);
    procLayout->setAlignment(Qt::AlignTop);
    procLayout->addWidget(grayScale);
    QWidget *procWidget = new QWidget;
    procWidget->setLayout(procLayout);
    toolBox->insertItem(1,procWidget,QStringLiteral("图像处理"));



    connect(loadImage,&QPushButton::clicked,this,&Detection::readImage);
    connect(loadVideo,&QPushButton::clicked,this,&Detection::readVideo);
    video = new VideoProcess();
    connect(video,&VideoProcess::newFrame,this,&Detection::showFrame);
}

Detection::~Detection()
{
    delete ui;
}

void Detection::paintEvent(QPaintEvent *event)
{
    if(!image->empty())
    {
        float ratio = image->cols*1.0 / image->rows;
        int _width = labelOri->width();
        int _height = labelOri->height();
        if(ratio>1.0)
        {
            cv::resize(*image,*image,cv::Size(_width,(int)(_width/ratio)));
        }
        else
        {
            cv::resize(*image,*image,cv::Size((int)(_height*ratio),_height));
        }
        int width = image->cols;
        int height = image->rows;
        QImage img(image->data,image->cols,image->rows,image->step,QImage::Format_RGB888);
        img.scaled(width,height,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        labelOri->setPixmap(QPixmap::fromImage(img.rgbSwapped()));
    }
}

void Detection::readImage()
{
    video->stopPlaying();
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,QStringLiteral("打开图片文件"),"E:/pics/",tr("(*.jpg;*.png)"));
    if(""!=fileName)
    {
        *image = cv::imread(fileName.toStdString(),cv::IMREAD_UNCHANGED);
        float ratio = image->cols*1.0 / image->rows;
        int _width = labelOri->width();
        int _height = labelOri->height();
        if(ratio>1.0)
        {
            cv::resize(*image,*image,cv::Size(_width,(int)(_width/ratio)));
        }
        else
        {
            cv::resize(*image,*image,cv::Size((int)(_height*ratio),_height));
        }
        int width = image->cols;
        int height = image->rows;
        QImage img(image->data,image->cols,image->rows,image->step,QImage::Format_RGB888);
        //img.scaled(width,height,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        labelOri->setPixmap(QPixmap::fromImage(img.rgbSwapped()));

        qDebug() << width << "\n" << height << "\n";
    }
}

void Detection::readVideo()
{
    if(!video->isFinished())
    {
        video->stopPlaying();
        video->quit();
        video = new VideoProcess();
        connect(video,&VideoProcess::newFrame,this,&Detection::showFrame);
    }
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,QStringLiteral("打开视频文件"),"G:/",tr("(*.mp4;*.avi)"));
    video->setVideoPath(fileName);
    video->start();
}

void Detection::showFrame(QPixmap frame)
{
    labelOri->setPixmap(frame);
}

//void Detection::mousePressEvent(QMouseEvent *event)
//{
//    int x_in_label = event->x();
//    int y_in_label = event->y();
//    qDebug() << "mouse_in_label: "<< x_in_label <<","<< y_in_label <<  "\n";
//}
