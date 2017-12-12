#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtGui>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QFileDialog *dial;
    QImage *pic;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openDialog();
    void saveDialog();
    void toGray();
    void dizering();
    void showImage();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
