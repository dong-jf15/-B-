#ifndef ROTATIONAL_DISTORTION_H
#define ROTATIONAL_DISTORTION_H
#include <QImage>
#include <QRgb>
using namespace std;

class rotational_distortion
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
    float radius;
    float angle;//角度制

    rotational_distortion();
    void get_rotational_distortion(QImage,Point2f,float,float);
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

#endif // ROTATIONAL_DISTORTION_H
