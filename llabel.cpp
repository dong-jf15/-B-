#include "llabel.h"
#include <QDebug>
llabel::llabel(QWidget * parent) :
QLabel(parent)
{

}



void llabel::mousePressEvent(QMouseEvent*event)
{
    qDebug()<<event->pos();
    if (event->button() == Qt::LeftButton)
        emit PixClicked(event->pos());
}

void llabel::mouseMoveEvent(QMouseEvent*event)
{
    if (event->buttons() & Qt::LeftButton)
        emit PixDraged(event->pos());
}

void llabel::mouseReleaseEvent(QMouseEvent*event)
{
    if (event->buttons() != Qt::LeftButton)
        emit release(event->pos());
}
