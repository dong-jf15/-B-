#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rotational_distortion.h"
#include "water_ripple.h"
#include "b_spline.h"
#include <string>
#include <QTimer>
#include <vector>
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>

#include "llabel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString image_address;
    QImage image_current;
    QTimer *timer;
    float fre;

    vector <QImage> history;
    B_spline::Point2f point_start;//B样条起始位置
    B_spline::Point2f point_end;//B样条终止为止
    int N_x;
    int N_y;

    llabel temp;//=new label(this);
private  slots:
    void open();
    void save();
    void quit();
    void source();
    void reset();
    void undo();
    //图上取点，确认圆心
    void CentralPoint(QPoint);
    void ReleasePoint(QPoint);

    void on_pushButton_RD_clicked();

    void on_pushButton_WR_clicked();

    void on_pushButton_BS_2_clicked();

private:
    Ui::MainWindow *ui;



};

#endif // MAINWINDOW_H
