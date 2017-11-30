#include "b_spline.h"

B_spline::B_spline()
{
    N_x = 50;
    N_y = 50;
}
//赋值函数
void B_spline::get_B_spline(QImage image,Point2f start,Point2f end,int Nx,int Ny)
{
    image_src = image.copy();
    image_dst = image.copy();
    point_start = start;
    point_end = end;
    N_x = Nx;
    N_y = Ny;
}
//三次B样条基函数
float B_spline::G_3(int i,float j)
{
    float Gij;
    switch (i)
    {
    case 0:
        Gij = 1.0 / 6.0*(3 * j*j - j*j*j - 3 * j + 1);
        break;
    case 1:
        Gij = 1.0 / 6.0*(3 * j*j*j - 6 * j*j + 4);
        break;
    case 2:
        Gij = 1.0 / 6.0*(3 * j*j - 3 * j*j*j + 3 * j + 1);
        break;
    case 3:
        Gij = 1.0 / 6.0*j*j*j;
        break;
    default:
        break;
    }
    return Gij;
}
//最近邻插值变换
void B_spline::transform_closest()
{
    float distance_x = point_end.x - point_start.x;
    float distance_y = point_end.x - point_start.x;
    distance_x = distance_x >N_x? N_x:distance_x;
    distance_x = distance_x <-N_x?-N_x:distance_x;
    distance_y = distance_y >N_y? N_y:distance_y;
    distance_y = distance_y <-N_y?-N_y:distance_y;
    for (int i = point_start.x - 2 * N_x + 1; i < point_start.x + 2 * N_x; i++)
    {
        if(i <0)
        {
            i = 0;
            continue;
        }
        if(i >= image_src.width())
            break;
        for (int j = point_start.y - 2 * N_y + 1; j < point_start.y + 2 * N_y; j++)
        {
            if(j <0)
            {
                j = 0;
                continue;
            }
            if(j >= image_src.height())
                break;
            int l = 1 - (int)floor((i - point_start.x) / N_x);
            int m = 1 - (int)floor((j - point_start.y) / N_y);
            float u = (float)l + 1.0*((float)i - point_start.x) / (float)N_x - 1;
            float v = (float)m + 1.0*((float)j - point_start.y) / (float)N_y - 1;
            float x0 = (float)i-(distance_x)*G_3(l, u)*G_3(m, v);
            float y0 = (float)j-(distance_y)*G_3(l, u)*G_3(m, v);

            x0 = x0 >= image_src.width()? image_src.width() - 1:x0;
            x0 = x0 <0? 0:x0;
            y0 = y0 >= image_src.height()? image_src.height() - 1:y0;
            y0 = y0 <0? 0:y0;
            Point2f point_src;
            point_src.x = x0;
            point_src.y = y0;
            image_dst.setPixel(QPoint(i,j),insert_closest(point_src));
        }
    }
}
//双线性插值
void B_spline::transform_bilinear()
{
    float distance_x = point_end.x - point_start.x;
    float distance_y = point_end.x - point_start.x;
    distance_x = distance_x >N_x? N_x:distance_x;
    distance_x = distance_x <-N_x?-N_x:distance_x;
    distance_y = distance_y >N_y? N_y:distance_y;
    distance_y = distance_y <-N_y?-N_y:distance_y;
    for (int i = point_start.x - 2 * N_x + 1; i < point_start.x + 2 * N_x; i++)
    {
        if(i <0)
        {
            i = 0;
            continue;
        }
        if(i >= image_src.width())break;
        for (int j = point_start.y - 2 * N_y + 1; j < point_start.y + 2 * N_y; j++)
        {
            if(j <0)
            {
                j = 0;
                continue;
            }
            if(j >=image_src.height())break;
            int l = 1 - (int)floor((i - point_start.x) / N_x);
            int m = 1 - (int)floor((j - point_start.y) / N_y);
            float u = (float)l + 1.0*((float)i - point_start.x) / (float)N_x - 1;
            float v = (float)m + 1.0*((float)j - point_start.y) / (float)N_y - 1;
            float x0 = (float)i-(distance_x)*G_3(l, u)*G_3(m, v);
            float y0 = (float)j-(distance_y)*G_3(l, u)*G_3(m, v);


            x0 = x0 >= image_src.width()? image_src.width() - 1:x0;
            x0 = x0 <0? 0:x0;
            y0 = y0 >= image_src.height()? image_src.height() - 1:y0;
            y0 = y0 <0? 0:y0;
            Point2f point_src;
            point_src.x = x0;
            point_src.y = y0;
            image_dst.setPixel(QPoint(i,j),insert_bilinear(point_src));
        }
    }
}
//双三次插值
void B_spline::transform_bicubic()
{
    float distance_x = point_end.x - point_start.x;
    float distance_y = point_end.x - point_start.x;
    distance_x = distance_x >N_x? N_x:distance_x;
    distance_x = distance_x <-N_x?-N_x:distance_x;
    distance_y = distance_y >N_y? N_y:distance_y;
    distance_y = distance_y <-N_y?-N_y:distance_y;
    for (int i = point_start.x - 2 * N_x + 1; i < point_start.x + 2 * N_x; i++)
    {
        if(i <0)
        {
            i = 0;
            continue;
        }
        if(i >= image_src.width())break;
        for (int j = point_start.y - 2 * N_y + 1; j < point_start.y + 2 * N_y; j++)
        {
            if(j <0)
            {
                j = 0;
                continue;
            }
            if(j >=image_src.height())break;
            int l = 1 - (int)floor((i - point_start.x) / N_x);
            int m = 1 - (int)floor((j - point_start.y) / N_y);
            float u = (float)l + 1.0*((float)i - point_start.x) / (float)N_x - 1;
            float v = (float)m + 1.0*((float)j - point_start.y) / (float)N_y - 1;
            float x0 = (float)i-(distance_x)*G_3(l, u)*G_3(m, v);
            float y0 = (float)j-(distance_y)*G_3(l, u)*G_3(m, v);


            x0 = x0 >= image_src.width()? image_src.width() - 1:x0;
            x0 = x0 <0? 0:x0;
            y0 = y0 >= image_src.height()? image_src.height() - 1:y0;
            y0 = y0 <0? 0:y0;
            Point2f point_src;
            point_src.x = x0;
            point_src.y = y0;
            image_dst.setPixel(QPoint(i,j),insert_bicubic(point_src));
        }
    }
}
QRgb B_spline::insert_closest(Point2f point_src)
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
QRgb B_spline::insert_bilinear(Point2f point_src)
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
float B_spline::S(float x)
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
float B_spline::get_value(int i,int j,int flag,QImage image)
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
QRgb B_spline::insert_bicubic(Point2f point_src)
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
