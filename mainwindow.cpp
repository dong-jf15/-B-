#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    timer->setInterval(50);
    connect(timer,SIGNAL(timeout()),this,SLOT(reset()));

    ui->label_image->setMouseTracking(true);
    connect(ui->label_image, SIGNAL(PixClicked(QPoint)), this, SLOT(CentralPoint(QPoint)));
    connect(ui->label_image, SIGNAL(release(QPoint)), this, SLOT(ReleasePoint(QPoint)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::CentralPoint(QPoint center)
{
    switch (ui->Option->currentIndex()) {
    case 0:
        {
        QString x_rotation = QString::number(center.x(),10);
        ui->lineEdit->setText(x_rotation);
        QString y_rotation = QString::number(center.y(),10);
        ui->lineEdit_2->setText(y_rotation);
        break;
        }
    case 1:
        {
        QString x_ripple = QString::number(center.x(),10);
        ui->lineEdit_5->setText(x_ripple);
        QString y_ripple = QString::number(center.y(),10);
        ui->lineEdit_6->setText(y_ripple);
        break;
        }
    case 2:
    {
        point_start.x = (float)center.x();
        point_start.y = (float)center.y();
        break;
    }
    default:
        break;
    }
}
void MainWindow::ReleasePoint(QPoint end)
{
    if(ui->Option->currentIndex() == 2)
    {
    timer->stop();
    point_end.x = end.x();
    point_end.y = end.y();
    if(point_end.x >=image_current.width()) point_end.x = image_current.width()-1;
    if(point_end.x <= 0) point_end.x = 0;
    if(point_end.y >=image_current.height()) point_end.y = image_current.height()-1;
    if(point_end.y <= 0) point_end.y = 0;
    B_spline changed;
    changed.get_B_spline(image_current,point_start,point_end,N_x,N_y);
    switch (ui->comboBox->currentIndex()) {
    case 0:
        changed.transform_closest();
        break;
    case 1:
        changed.transform_bilinear();
        break;
    case 2:
        changed.transform_bicubic();
    default:
        break;
    }
    history.push_back(image_current);
    image_current = changed.image_dst;
    ui->label_image->setPixmap(QPixmap::fromImage(image_current));
    }
}
void MainWindow::open()
{
    timer->stop();
    QString file_address = QFileDialog::getOpenFileName(this,"打开图片",NULL,"图片文件(*.jpg *.bmp *.jpeg *.png)");
    image_address = file_address;
    QImage image_src = QImage(file_address);
    image_current = image_src;
    ui->label_image->resize(image_src.height(),image_src.width());
    ui->label_image->setPixmap(QPixmap::fromImage(image_current));
}
void MainWindow::save()
{
    timer->stop();
    if(ui->label_image->isEnabled())
    {
    QString filename = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images (*.png *.bmp *.jpg)"));
    image_current.save(filename);
    }
    else QMessageBox::information(NULL, QString("提示"), QString("您还未读取图片"));
}

void MainWindow::quit()
{
    timer->stop();
}
void MainWindow::undo()
{
    if(!history.empty())
    {
        image_current = history.back();
        history.pop_back();
        ui->label_image->resize(image_current.height(),image_current.width());
        ui->label_image->setPixmap(QPixmap::fromImage(image_current));
    }
}

void MainWindow::source()
{
    timer->stop();
    QImage image_src = QImage(image_address);
    history.push_back(image_current);
    image_current = image_src;
    ui->label_image->resize(image_current.height(),image_current.width());
    ui->label_image->setPixmap(QPixmap::fromImage(image_src));
}

void MainWindow::on_pushButton_RD_clicked()
{
    timer->stop();
    QImage image_src;
    rotational_distortion::Point2f point_center;
    float r;
    float angle;
    image_src = image_current;
    point_center.x = ui->lineEdit->text().toFloat();
    point_center.y = ui->lineEdit_2->text().toFloat();
    r = ui->lineEdit_3->text().toFloat();
    angle = ui->lineEdit_4->text().toFloat();
    rotational_distortion changed;
    changed.get_rotational_distortion(image_src,point_center,r,angle);
    switch (ui->comboBox->currentIndex()) {
    case 0:
        changed.transform_closest();
        break;
    case 1:
        changed.transform_bilinear();
        break;
    case 2:
        changed.transform_bicubic();
    default:
        break;
    }
    history.push_back(image_current);
    image_current = changed.image_dst;
    ui->label_image->resize(image_current.height(),image_current.width());
    ui->label_image->setPixmap(QPixmap::fromImage(image_current));
}

void MainWindow::on_pushButton_WR_clicked()
{
    timer->stop();
    QImage image_src;
    water_ripple::Point2f point_center;
    float p;
    float q;
    float radius;
    int flag;

    image_src = image_current;
    point_center.x = ui->lineEdit_5->text().toFloat();
    point_center.y = ui->lineEdit_6->text().toFloat();
    p = ui->lineEdit_7->text().toFloat();
    q = ui->lineEdit_8->text().toFloat();
    radius = ui->lineEdit_9->text().toFloat();
    flag = ui->checkBox->isChecked();
    water_ripple changed;
    switch (flag) {
    case 0:
        changed.get_water_ripple(image_src,point_center,p,q,radius);
        switch (ui->comboBox->currentIndex()) {
        case 0:
            changed.transform_closest();
            break;
        case 1:
            changed.transform_bilinear();
            break;
        case 2:
            changed.transform_bicubic();
            break;
        default:
            break;
        }
        history.push_back(image_current);
        image_current = changed.image_dst;
        ui->label_image->resize(image_current.height(),image_current.width());
        ui->label_image->setPixmap(QPixmap::fromImage(image_current));
        break;
    case 1:
        changed.get_water_ripple(image_src,point_center,p,q,radius);
        switch (ui->comboBox->currentIndex()) {
        case 0:
            changed.transform_closest();
            break;
        case 1:
            changed.transform_bilinear();
            break;
        case 2:
            changed.transform_bicubic();
            break;
        default:
            break;
        }
        history.push_back(image_current);
        image_current = changed.image_dst;
        ui->label_image->resize(image_current.height(),image_current.width());
        ui->label_image->setPixmap(QPixmap::fromImage(image_current));
        fre = q;
        timer->start();
        break;
    default:
        break;
    }
}
void MainWindow::reset()
{
    QImage image_src = image_current;
    water_ripple::Point2f point_center;
    point_center.x = ui->lineEdit_5->text().toFloat();
    point_center.y = ui->lineEdit_6->text().toFloat();
    float p = ui->lineEdit_7->text().toFloat();
    float radius = ui->lineEdit_9->text().toFloat();
    fre = fre + 10;
    water_ripple wr;
    wr.get_water_ripple(image_src,point_center,p,fre,radius);
    switch (ui->comboBox->currentIndex()) {
    case 0:
        wr.transform_closest();
        break;
    case 1:
        wr.transform_bilinear();
        break;
    case 2:
        wr.transform_bicubic();
        break;
    default:
        break;
    }
    ui->label_image->resize(image_current.height(),image_current.width());
    ui->label_image->setPixmap(QPixmap::fromImage(wr.image_dst));
}

void MainWindow::on_pushButton_BS_2_clicked()
{
    timer->stop();
    N_x = ui->lineEdit_10->text().toInt();
    N_y = ui->lineEdit_11->text().toInt();
}

