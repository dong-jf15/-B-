#ifndef WATER_RIPPLE_H
#define WATER_RIPPLE_H
#include <QImage>
#include <QRgb>

using namespace std;
class water_ripple
{
public:
    struct Point2f
    {
        float x;
        float y;
    };
    QImage image_src;
    QImage image_dst;
    Point2f center;
    float p;
    float q;
    float radius;

    water_ripple();
    void get_water_ripple(QImage,Point2f,float,float,float);
    Point2f affine(Point2f);
    QRgb insert_closest(Point2f);
    QRgb insert_bilinear(Point2f);
    QRgb insert_bicubic(Point2f);
    float S(float);
    bool in_circle(Point2f);
    float get_value(int,int,int,QImage);
    void transform_closest();
    void transform_bilinear();
    void transform_bicubic();
};

#endif // WATER_RIPPLE_H
