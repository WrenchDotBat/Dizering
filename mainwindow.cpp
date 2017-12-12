#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dial = new QFileDialog(this);
    pic = new QImage;
    dial->setFileMode(QFileDialog::ExistingFile);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(openDialog()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(saveDialog()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(toGray()));
    connect(ui->dizeringButton, SIGNAL(clicked()), this, SLOT(dizering()));
}

void MainWindow::showImage(){
    ui->PictureBox->setGeometry(20, 30, pic->width(), pic->height());
    ui->PictureBox->setPixmap(QPixmap::fromImage(*pic));
}

void MainWindow::dizering(){
    int err = 0;
    for (int i = 0; i < pic->height(); i++)
        for (int j = 0; j < pic->width(); j++)
            if (pic->pixelColor(j, i).red() + err < 127){
                err = pic->pixelColor(j, i).red();
                pic->setPixelColor(j, i, QColor(0, 0, 0));
            } else {
                err = pic->pixelColor(j, i).red() - 255;
                pic->setPixelColor(j, i, QColor(255, 255, 255));
            }
    showImage();
}

void MainWindow::toGray(){
    //QPainter *painter = new QPainter(pic);
    QColor tempColor;
    int temp;
    for (int i = 0; i < pic->height(); i++)
        for (int j = 0; j < pic->width(); j++){
            tempColor = pic->pixelColor(j, i);
            temp = (tempColor.red() + tempColor.blue() + tempColor.green()) / 3;
            //tempColor = 128;
            pic->setPixelColor(j, i, QColor(temp, temp, temp));
        }
    //painter->drawRect(40, 40, 200, 200);
    showImage();
}

void MainWindow::saveDialog(){
    pic->save(QFileDialog::getSaveFileName(), "JPG");
    showImage();
}

void MainWindow::openDialog(){
    pic->load(dial->getOpenFileName());
    showImage();
}

MainWindow::~MainWindow()
{
    delete ui;
}
