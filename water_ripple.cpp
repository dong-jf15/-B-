#include "water_ripple.h"

water_ripple::water_ripple()
{

}
void water_ripple::get_water_ripple(QImage img_src,Point2f centre,float a,float b,float r)
{
    image_src = img_src.copy();
    image_dst = img_src.copy();
    center = centre;
    p = a;
    q = b;
    radius = r;
}
//仿射函数，求出目标图中对应原图中的点
water_ripple::Point2f water_ripple::affine(Point2f point_dst)
{
    //相对于旋转中心点的坐标
    Point2f point_temp;
    point_temp.x = point_dst.x-center.x;
    point_temp.y = point_dst.y-center.y;
    //求半径和角度
    float r = sqrt(point_temp.x*point_temp.x + point_temp.y*point_temp.y);
    float angle_a = atan2f(point_temp.y,point_temp.x);
    float arcq = q*3.1415926/180;

    Point2f point_src;
    point_src.x = r*cosf(angle_a + 0.1*sinf(r/radius*p + arcq)) + center.x;
    point_src.y = r*sinf(angle_a + 0.1*sinf(r/radius*p + arcq)) + center.y;

    //判断是否已经出界并处理出界网格点
    point_src.x = point_src.x >= image_src.width()? image_src.width() - 1:point_src.x;
    point_src.x = point_src.x < 0? 0:point_src.x;
    point_src.y = point_src.y >= image_src.height()? image_src.height() - 1:point_src.y;
    point_src.y = point_src.y < 0? 0:point_src.y;

    return point_src;
}
//最近邻插值
QRgb water_ripple::insert_closest(Point2f point_src)
{
    Point2f point_temp;
    //四舍五入求最近邻点
    if((point_src.x - floor(point_src.x)) >= 0.5)
    {
        point_temp.x = ceil(point_src.x);
    }
    else
    {
        point_temp.x = floor(point_src.x);
    }

    if((point_src.y - floor(point_src.y)) >= 0.5)
    {
        point_temp.y = ceil(point_src.y);
    }
    else
    {
        point_temp.y = floor(point_src.y);
    }
    point_temp.x = point_temp.x >= image_src.width()? image_src.width() - 1:point_temp.x;
    point_temp.y = point_temp.y >= image_src.height()? image_src.height() - 1:point_temp.y;

    QPoint point;
    point.setX(point_temp.x);
    point.setY(point_temp.y);

    return image_src.pixel(point);
}
//双线性插值
QRgb water_ripple::insert_bilinear(Point2f point_src)
{
    point_src.x = point_src.x >= image_src.width()? image_src.width() - 1:point_src.x;
    point_src.y = point_src.y >= image_src.height()? image_src.height() - 1:point_src.y;
    //求出该点所处网格的四个顶点
    QPoint Point1;
    Point1.setX(floor(point_src.x));
    Point1.setY(floor(point_src.y));
    QPoint Point2;
    Point2.setX(floor(point_src.x));
    Point2.setY(ceil(point_src.y));
    QPoint Point3;
    Point3.setX(ceil(point_src.x));
    Point3.setY(floor(point_src.y));
    QPoint Point4;
    Point4.setX(ceil(point_src.x));
    Point4.setY(ceil(point_src.y));
    //求出网格点像素值
    QRgb value1 = image_src.pixel(Point1);
    QRgb value2 = image_src.pixel(Point2);
    QRgb value3 = image_src.pixel(Point3);
    QRgb value4 = image_src.pixel(Point4);
    //根据双线性公式插值求出RGB值
    float x= point_src.x-floor(point_src.x);
    float y= point_src.y-floor(point_src.y);
    float value_red = ((float)qRed(value1)*(1-x)+(float)qRed(value3)*x)*(1-y)+
            ((float)qRed(value2)*(1-x)+(float)qRed(value4)*x)*y;
    float value_green = ((float)qGreen(value1)*(1-x)+(float)qGreen(value3)*x)*(1-y)+
            ((float)qGreen(value2)*(1-x)+(float)qGreen(value4)*x)*y;
    float value_blue = ((float)qBlue(value1)*(1-x)+(float)qBlue(value3)*x)*(1-y)+
            ((float)qBlue(value2)*(1-x)+(float)qBlue(value4)*x)*y;

    QRgb value = qRgb((int)value_red,(int)value_green,(int)value_blue);
    return value;
}
//三次插值核函数
float water_ripple::S(float x)
{
    float y = fabs(x);
    float S;
    if(y <= 1)
    {
        S = 1 - 2*y*y + y*y*y;
    }
    else if(y > 1 && y < 2)
    {
        S = 4 - 8*y + 5*y*y -y*y*y;
    }
    else
    {
        S =0;
    }

    return S;
}
//获取像素点的值
float water_ripple::get_value(int i,int j,int flag,QImage image)
{
    i = i>=image.width()? image.width()-1:i;
    i = i< 0? 0:i;
    j = j>=image.height()? image.height()-1:j;
    j = j< 0? 0:j;
    QPoint qpoint(i,j);
    switch(flag)
    {
    case 1://red
        return (float)qRed(image.pixel(qpoint));
        break;
    case 2://green
        return (float)qGreen(image.pixel(qpoint));
        break;
    case 3://blue
        return (float)qBlue(image.pixel(qpoint));
        break;
    default:
        return 0;
        break;
    }
}
//双三次插值
QRgb water_ripple::insert_bicubic(Point2f point_src)
{
    float u = point_src.x - floor(point_src.x);
    float v = point_src.y - floor(point_src.y);
    int i = (int)floor(point_src.x);
    int j = (int)floor(point_src.y);

    //分为RGB分别插值
    float temp1_red = S(v+1)*get_value(i-1,j-1,1,image_src)+S(v)*get_value(i-1,j,1,image_src)+
        S(v - 1)*get_value(i-1,j+1,1,image_src)+S(v - 2)*get_value(i-1,j+2,1,image_src);
    float temp1_green = S(v+1)*get_value(i-1,j-1,2,image_src)+S(v)*get_value(i-1,j,2,image_src)+
            S(v - 1)*get_value(i-1,j+1,2,image_src)+S(v - 2)*get_value(i-1,j+2,2,image_src);
    float temp1_blue = S(v+1)*get_value(i-1,j-1,3,image_src)+S(v)*get_value(i-1,j,3,image_src)+
            S(v - 1)*get_value(i-1,j+1,3,image_src)+S(v - 2)*get_value(i-1,j+2,3,image_src);

    float temp2_red = S(v+1)*get_value(i,j-1,1,image_src)+S(v)*get_value(i,j,1,image_src)+
        S(v - 1)*get_value(i,j+1,1,image_src)+S(v - 2)*get_value(i,j+2,1,image_src);
    float temp2_green = S(v+1)*get_value(i,j-1,2,image_src)+S(v)*get_value(i,j,2,image_src)+
            S(v - 1)*get_value(i,j+1,2,image_src)+S(v - 2)*get_value(i,j+2,2,image_src);
    float temp2_blue = S(v+1)*get_value(i,j-1,3,image_src)+S(v)*get_value(i,j,3,image_src)+
            S(v - 1)*get_value(i,j+1,3,image_src)+S(v - 2)*get_value(i,j+2,3,image_src);

    float temp3_red = S(v+1)*get_value(i+1,j-1,1,image_src)+S(v)*get_value(i+1,j,1,image_src)+
        S(v - 1)*get_value(i+1,j+1,1,image_src)+S(v - 2)*get_value(i+1,j+2,1,image_src);
    float temp3_green = S(v+1)*get_value(i+1,j-1,2,image_src)+S(v)*get_value(i+1,j,2,image_src)+
            S(v - 1)*get_value(i+1,j+1,2,image_src)+S(v - 2)*get_value(i+1,j+2,2,image_src);
    float temp3_blue = S(v+1)*get_value(i+1,j-1,3,image_src)+S(v)*get_value(i+1,j,3,image_src)+
            S(v - 1)*get_value(i+1,j+1,3,image_src)+S(v - 2)*get_value(i+1,j+2,3,image_src);

    float temp4_red = S(v+1)*get_value(i+2,j-1,1,image_src)+S(v)*get_value(i+2,j,1,image_src)+
        S(v - 1)*get_value(i+2,j+1,1,image_src)+S(v - 2)*get_value(i+2,j+2,1,image_src);
    float temp4_green = S(v+1)*get_value(i+2,j-1,2,image_src)+S(v)*get_value(i+2,j,2,image_src)+
            S(v - 1)*get_value(i+2,j+1,2,image_src)+S(v - 2)*get_value(i+2,j+2,2,image_src);
    float temp4_blue = S(v+1)*get_value(i+2,j-1,3,image_src)+S(v)*get_value(i+2,j,3,image_src)+
            S(v - 1)*get_value(i+2,j+1,3,image_src)+S(v - 2)*get_value(i+2,j+2,3,image_src);

    float value_red = temp1_red*S(u+1) + temp2_red*S(u) + temp3_red*S(u-1) + temp4_red*S(u-2);
    float value_blue = temp1_blue*S(u+1) + temp2_blue*S(u) + temp3_blue*S(u-1) + temp4_blue*S(u-2);
    float value_green = temp1_green*S(u+1) + temp2_green*S(u) + temp3_green*S(u-1) + temp4_green*S(u-2);

    //判断像素值是否出界
    value_red = value_red >255? 255:value_red;
    value_green = value_green >255? 255:value_green;
    value_blue = value_blue >255? 255:value_blue;
    value_red = value_red <0? 0:value_red;
    value_green = value_green <0? 0:value_green;
    value_blue = value_blue <0? 0:value_blue;

    QRgb value = qRgb((int)value_red,(int)value_green,(int)value_blue);
    return value;

}
//判断是否在扭曲圆中
bool water_ripple::in_circle(Point2f point_dst)
{
    float x = point_dst.x - center.x;
    float y = point_dst.y - center.y;
    float distance = sqrt(x*x + y*y);
    if(distance > radius)
    {
        return false;
    }
    else
        return true;
}

//最近邻插值变换
void water_ripple::transform_closest()
{
    Point2f point_dst;
    Point2f point_src;
    for(int i=0;i < image_dst.width();i++)
    {
        for(int j=0;j < image_src.height();j++)
        {
            point_dst.x = i;
            point_dst.y = j;
            if(in_circle(point_dst))
            {
                QPoint qpoint(i,j);
                point_src = affine(point_dst);
                image_dst.setPixel(qpoint ,insert_closest(point_src));
            }
        }
    }
}
//双线性插值变换
void water_ripple::transform_bilinear()
{
    Point2f point_dst;
    Point2f point_src;
    QPoint qpoint;
    for(int i=0;i < image_dst.width();i++)
    {
        for(int j=0;j < image_src.height();j++)
        {
            point_dst.x = i;
            point_dst.y = j;
            if(in_circle(point_dst))
            {
                qpoint.setX(i);
                qpoint.setY(j);
                point_src = affine(point_dst);
                image_dst.setPixel(qpoint ,insert_bilinear(point_src));
            }
        }
    }
}
//双三次插值变换
void water_ripple::transform_bicubic()
{
    Point2f point_dst;
    Point2f point_src;
    QPoint qpoint;
    for(int i=0;i < image_dst.width();i++)
    {
        for(int j=0;j < image_src.height();j++)
        {
            point_dst.x = i;
            point_dst.y = j;
            if(in_circle(point_dst))
            {
                qpoint.setX(i);
                qpoint.setY(j);
                point_src = affine(point_dst);
                image_dst.setPixel(qpoint ,insert_bicubic(point_src));
            }
        }
    }
}
