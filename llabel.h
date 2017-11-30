#ifndef LLABEL_H
#define LLABEL_H
#include <QLabel>
#include <QMouseEvent>

class llabel : public QLabel {
    Q_OBJECT

public:
    llabel(QWidget * parent = 0);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

signals:
    void PixClicked(QPoint);
    void PixDraged(QPoint);
    void release(QPoint);

private:

};
#endif // LLABEL_H
