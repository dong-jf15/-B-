#ifndef B_SPLINE_H
#define B_SPLINE_H

#include <QImage>
#include <QRgb>
class B_spline
{
public:
    struct Point2f
    {
        float x;
        float y;
    };
    QImage image_src;
    QImage image_dst;
    Point2f point_start;
    Point2f point_end;
    int N_x;
    int N_y;

    B_spline();
    void get_B_spline(QImage,Point2f,Point2f,int,int);
    float G_3(int,float);
    void transform_closest();
    void transform_bilinear();
    void transform_bicubic();
    QRgb insert_closest(Point2f);
    QRgb insert_bilinear(Point2f);
    QRgb insert_bicubic(Point2f);
    float S(float);
    float get_value(int,int,int,QImage);
};
#endif // B_SPLINE_H
