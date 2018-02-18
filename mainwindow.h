#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtGui>

struct DizerPoint;

namespace Ui {
class MainWindow;
}

class DizeringMatrix {
public:
    DizerPoint *values;
    int length;
    int divider;
    DizeringMatrix(int);
    ~DizeringMatrix();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QFileDialog *dial;
    QImage *pic;
    QImage *backupPic;
    QColor MixColors(QColor*, QColor*);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openDialog();
    void saveDialog();
    void toGray();
    void dizering();
    void noDizeringApprox();
    void simpleDizering();
    void smoothDizering(DizeringMatrix*);
    void showImage(QImage*);
    int getColor(int, int, int);

private slots:
    void on_resetButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

