#include "mainwindow.h"
#include "ui_mainwindow.h"

struct DizerPoint{
    int x, y, coefficient;
};

DizeringMatrix::DizeringMatrix(int size = 4){
    length = size;
    values = new DizerPoint[size];
}

DizeringMatrix::~DizeringMatrix(){
    delete[] values;
}

void MainWindow::smoothDizering(DizeringMatrix* matrix){
    on_resetButton_clicked();
    if (ui->isGrayCheckbox->isChecked())
        toGray();
    int tempX, tempY, colorCount = 3;
    double error;
    int* colorComponent = new int(colorCount);
    QMap<QPair<int,int>, double*> errMap;
    errMap[qMakePair(0, 0)] = new double(colorCount);
    for (int i = 0; i < colorCount; i++)
        errMap[qMakePair(0, 0)][i] = 0;

    for (int i = 0; i < pic->width(); i++)
        for (int j = 0; j < pic->height(); j++){
            qDebug() << "pixel " << i << " " << j << '\n';
            for (int c = 0; c < colorCount; c++){
                // qDebug() << "color " << (c + 1) << '\n';
                error = getColor(i, j, c);
                if (errMap.contains(qMakePair(i, j)))
                    error += errMap[qMakePair(i, j)][c];
                if (qRound(error) < 127){
                    colorComponent[c] = 0;
                } else {
                    colorComponent[c] = 255;
                    error -= 255;
                }
                for (int k = 0; k < matrix->length; k++){
                    tempX = matrix->values[k].x + i;
                    tempY = matrix->values[k].y + j;
                    if (tempX > 0 && tempY > 0 && tempX < pic->width() && tempY < pic->height()){
                        if (errMap.contains(qMakePair(tempX, tempY)))
                            errMap[qMakePair(tempX, tempY)][c] += matrix->values[k].coefficient * error / matrix->divider;
                        else{
                            errMap[qMakePair(tempX, tempY)] = new double(colorCount);
                            for (int u = 0; u < colorCount; u++)
                                errMap[qMakePair(tempX, tempY)][u] = 0;
                            errMap[qMakePair(tempX, tempY)][c] = matrix->values[k].coefficient * error / matrix->divider;
                        }
                    }
                }
            }
            //delete[] errMap[qMakePair(i, j)];
            errMap.remove(qMakePair(i, j));
            pic->setPixelColor(i, j, QColor(colorComponent[0], colorComponent[1], colorComponent[2]));
        }

    showImage(pic);
}

QColor MainWindow::MixColors(QColor* a, QColor* b){
    QColor tempColor;
    if (a->red() + b->red() > 255) tempColor.setRed(255);
    else tempColor.setRed(a->red() + b->red());
    if (a->green() + b->green() > 255) tempColor.setGreen(255);
    else tempColor.setGreen(a->green() + b->green());
    if (a->blue() + b->blue() > 255) tempColor.setBlue(255);
    else tempColor.setBlue(a->blue() + b->blue());
    return tempColor;
}

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dial = new QFileDialog(this);
    pic = new QImage;
    backupPic = new QImage;
    dial->setFileMode(QFileDialog::ExistingFile);
    ui->rgbRadioButton->setChecked(true);
    connect(ui->openButton, SIGNAL(clicked()), this, SLOT(openDialog()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveDialog()));
    connect(ui->dizeringButton, SIGNAL(clicked()), this, SLOT(dizering()));
}

void MainWindow::showImage(QImage *img){
    ui->PictureBox->resize(img->width(), img->height());
    ui->PictureBox->setPixmap(QPixmap::fromImage(*img));
}

void MainWindow::dizering(){
    if (ui->isSimple->isChecked()){
        simpleDizering();
        return;
    }
    else if (ui->isFloyd->isChecked()){
        DizeringMatrix matrix(4);
        matrix.values[0].x = 1;
        matrix.values[0].y = 0;
        matrix.values[0].coefficient = 7;

        matrix.values[1].x = 1;
        matrix.values[1].y = 1;
        matrix.values[1].coefficient = 1;

        matrix.values[2].x = 0;
        matrix.values[2].y = 1;
        matrix.values[2].coefficient = 5;

        matrix.values[3].x = -1;
        matrix.values[3].y = 1;
        matrix.values[3].coefficient = 3;

        matrix.divider = 16;
        smoothDizering(&matrix);
    }
    else if (ui->noDizer->isChecked()){
        noDizeringApprox();
        return;
    }
    else if (ui->isJarvis->isChecked()){
        DizeringMatrix matrix(12);
        matrix.values[0].x = 0;
        matrix.values[0].y = 1;
        matrix.values[0].coefficient = 7;

        matrix.values[1].x = 0;
        matrix.values[1].y = 2;
        matrix.values[1].coefficient = 5;
        int counter = 2;

    for (int i = 1; i < 3; i++)
        for (int j = -2; j < 3; j++){
            matrix.values[counter].x = j;
            matrix.values[counter].y = i;
            matrix.values[counter].coefficient = 1 + 2 * (4 - qAbs(j) - i);
            counter++;
        }
    matrix.divider = 48;
    smoothDizering(&matrix);
    }
}

int MainWindow::getColor(int j, int i, int colorComponent = 0){
    switch(colorComponent){
    case 0: return pic->pixelColor(j, i).red();
    case 1: return pic->pixelColor(j, i).green();
    default : return pic->pixelColor(j, i).blue();
    }
}


void MainWindow::noDizeringApprox(){
    int colorCounter = 3;
    if (ui->isGrayCheckbox->isChecked()){
        toGray();
        colorCounter = 1;
    }
    int *colorArr = new int(colorCounter);
    for (int i = 0; i < pic->width(); i++)
        for (int j = 0; j < pic->height(); j++){
            for (int k = 0; k < colorCounter; k++)
                if (getColor(i, j, k) < 127)
                    colorArr[k] = 0;
                else
                    colorArr[k] = 255;
            if (ui->isGrayCheckbox->isChecked())
                pic->setPixelColor(i, j, QColor(colorArr[0], colorArr[0], colorArr[0]));
            else
                pic->setPixelColor(i, j, QColor(colorArr[0], colorArr[1], colorArr[2]));
        }
    showImage(pic);
}

void MainWindow::simpleDizering(){
    on_resetButton_clicked();
    int colorCounter = 3, *errArr, *colorArr;
    if (ui->isGrayCheckbox->isChecked()){
        toGray();
        colorCounter = 1;
    }
    errArr = new int(colorCounter);
    colorArr = new int(colorCounter);
    for (int i = 0; i < pic->width(); i++)
        for (int j = 0; j < pic->height(); j++){
            for (int k = 0; k < colorCounter; k++)
                if (getColor(i, j, k) + errArr[k] < 127){
                    errArr[k] = getColor(i, j, k);
                    colorArr[k] = 0;
                } else {
                    errArr[k] = getColor(i, j, k) - 255;
                    colorArr[k] = 255;
                }
            if (ui->isGrayCheckbox->isChecked())
                pic->setPixelColor(i, j, QColor(colorArr[0], colorArr[0], colorArr[0]));
            else
                pic->setPixelColor(i, j, QColor(colorArr[0], colorArr[1], colorArr[2]));
        }
    showImage(pic);
}

void MainWindow::toGray(){
    //QPainter *painter = new QPainter(pic);
    QColor tempColor;
    int temp;
    for (int i = 0; i < pic->height(); i++)
        for (int j = 0; j < pic->width(); j++){
            tempColor = pic->pixelColor(j, i);
            if (ui->rgbRadioButton->isChecked())
                temp = (tempColor.red() + tempColor.blue() + tempColor.green()) / 3;
            else if (ui->hsvRadioButton->isChecked()){
                if (tempColor.red() > tempColor.green() && tempColor.red() > tempColor.blue())
                    temp = tempColor.red();
                else if (tempColor.red() < tempColor.green() && tempColor.red() > tempColor.blue())
                    temp = tempColor.green();
                else
                    temp = tempColor.blue();
            }
            //tempColor = 128;
            pic->setPixelColor(j, i, QColor(temp, temp, temp));
        }
    //painter->drawRect(40, 40, 200, 200);
    showImage(pic);
}

void MainWindow::saveDialog(){
    pic->save(QFileDialog::getSaveFileName(), "JPG");
    showImage(pic);
}

void MainWindow::openDialog(){
    if (pic->load(dial->getOpenFileName())){
        *backupPic = pic->copy();
        showImage(pic);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_resetButton_clicked()
{
    showImage(backupPic);
    *pic = backupPic->copy();
}
