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
    toolBox->setMinimumWidth(140);

    //toolBox's container
    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(toolBox);
    widgetLayout->setMargin(0);
    ui->widget->setLayout(widgetLayout);

    //container for main window of piture or video
    QVBoxLayout *vLayout = new QVBoxLayout;
    labelOri = new QLabel(this);
    labelOri->setMaximumSize(widthLabelOri,heightLabelOri);
    labelOri->setMinimumSize(600,450);
    vLayout->addWidget(labelOri);
    labelOriScrollBar = new QScrollBar;
    labelOriScrollBar->setOrientation(Qt::Horizontal);
    vLayout->addWidget(labelOriScrollBar);

    //container for threee side windows
    QVBoxLayout *labelsVLayout = new QVBoxLayout;
    labelROI = new QLabel(this);
    //labelROI->setMaximumWidth(200);
    labelROI->setMinimumWidth(200);
    labelsVLayout->addWidget(labelROI);
    QLabel *labelHistogram = new QLabel(this);
    //labelHistogram->setMaximumWidth(200);
    labelHistogram->setMinimumWidth(200);
    labelsVLayout->addWidget(labelHistogram);
    QLabel *labelDetecion = new QLabel(this);
    //labelDetecion->setMaximumWidth(200);
    labelDetecion->setMinimumWidth(200);
    labelsVLayout->addWidget(labelDetecion);
    //container for above two containers
    QHBoxLayout *hMainLayout = new QHBoxLayout;
    hMainLayout->addLayout(vLayout);
    hMainLayout->addLayout(labelsVLayout);

    ui->groupBox->setLayout(hMainLayout);

    QPushButton *loadImage = new QPushButton(QStringLiteral("Load Image"),this);
    QPushButton *loadVideo = new QPushButton(QStringLiteral("Load Video"),this);
    videoGrayCheckBox = new QCheckBox;
    videoGrayCheckBox->setText("If load gray");
    videoGrayCheckBox->setCheckState(Qt::CheckState::Unchecked);
    QVBoxLayout *loadLayout = new QVBoxLayout;
    loadLayout->setMargin(0);
    loadLayout->addWidget(loadImage);
    loadLayout->addWidget(videoGrayCheckBox);
    loadLayout->addWidget(loadVideo);
    loadLayout->setAlignment(Qt::AlignTop);
    QWidget *loadWidget = new QWidget;
    loadWidget->setLayout(loadLayout);
    toolBox->insertItem(0,loadWidget,QStringLiteral("Load"));

    QPushButton *grayScale = new QPushButton(QStringLiteral("grayscale"),this);
    QVBoxLayout *procLayout = new QVBoxLayout;
    procLayout->setMargin(0);
    procLayout->setAlignment(Qt::AlignTop);
    procLayout->addWidget(grayScale);
    QWidget *procWidget = new QWidget;
    procWidget->setLayout(procLayout);
    toolBox->insertItem(1,procWidget,QStringLiteral("Process"));

    connect(loadImage,&QPushButton::clicked,this,&Detection::readImage);
    connect(loadVideo,&QPushButton::clicked,this,&Detection::readVideo);
    connect(grayScale,&QPushButton::clicked,this,&Detection::grayScale);
    video = new VideoProcess();
    connect(video,&VideoProcess::newFrame,this,&Detection::showFrame);
    connect(this,&Detection::cvtGray,video,&VideoProcess::cvtGray);
    connect(video,&VideoProcess::currentFrameInfo,this,&Detection::getCurrentFrameInfo);
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
    if(video!=nullptr && !video->isFinished())
    {
        video->stopPlaying();
    }
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,QStringLiteral("Open picture file"),"E:/pics/",tr("(*.jpg;*.png)"));
    if(""!=fileName)
    {
        *image = cv::imread(fileName.toStdString(),cv::IMREAD_COLOR);
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
//        int width = image->cols;
//        int height = image->rows;
//        img.scaled(width,height,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        labelOri->setPixmap(QPixmap::fromImage(QImage(image->data,image->cols,image->rows,image->step,QImage::Format_BGR888)));
    }
}
void Detection::readVideo()
{
    if(!video->isFinished())
    {
        setted = false;
        video->stopPlaying();
        video->quit();
        video = new VideoProcess();
        connect(video,&VideoProcess::newFrame,this,&Detection::showFrame);
        connect(this,&Detection::cvtGray,video,&VideoProcess::cvtGray);
        connect(video,&VideoProcess::currentFrameInfo,this,&Detection::getCurrentFrameInfo);
        connect(labelOriScrollBar,&QScrollBar::sliderMoved,video,&VideoProcess::setPosFrame);
    }
    if(videoGrayCheckBox->checkState())
    {
        emit cvtGray(true);
    }
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,QStringLiteral("Open video file"),"G:/",tr("(*.mp4;*.avi)"));
    video->setVideoPath(fileName);
    video->start();
}
void Detection::grayScale()
{
    if(!image->empty() && image->channels()>1)
    {
        cv::Mat tmp;
        cv::cvtColor(*image,tmp,CV_BGR2GRAY);
        QImage img(tmp.data,tmp.cols,tmp.rows,tmp.step, QImage::Format_Grayscale8);
        labelROI->setPixmap(QPixmap::fromImage(img));
    }
}
void Detection::showFrame(QPixmap frame)
{
    isPlaying = true;
    labelOri->setPixmap(frame);
//    QImage tmp = frame.toImage();
//    tmp = tmp.convertToFormat(QImage::Format_Grayscale8);
//    labelROI->setPixmap(QPixmap::fromImage(tmp));
}

QImage Detection::cvtGrayScale(QImage image)
{
    cv::Mat tmp(image.height(),image.width(),CV_8UC3,image.bits(),image.bytesPerLine());
    cv::cuda::GpuMat d_frame;
    d_frame.upload(tmp);
    cv::cuda::cvtColor(d_frame,d_frame,CV_BGR2GRAY);
    d_frame.download(tmp);
    return QImage(tmp.data,tmp.cols,tmp.rows,tmp.step,QImage::Format_Grayscale8);
}
void Detection::getCurrentFrameInfo(int frameCount, int posFrame)
{
    if(!setted)
    {
        labelOriScrollBar->setRange(0,frameCount);
        setted = true;
    }
    labelOriScrollBar->setValue(posFrame);
}
//void Detection::mousePressEvent(QMouseEvent *event)
//{
//    int x_in_label = event->x();
//    int y_in_label = event->y();
//    qDebug() << "mouse_in_label: "<< x_in_label <<","<< y_in_label <<  "\n";
//}
