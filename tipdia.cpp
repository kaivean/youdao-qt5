#include "tipdia.h"
#include <QtGui>



tipDia::tipDia(QWidget *parent) :
    QDialog(parent)
{

    QPixmap pixmap(":/images/ok.png");
    QPixmap pix= pixmap.scaled(35,35);

    QPalette   palette;
    palette.setBrush(backgroundRole(),QBrush(pix));
    setPalette(palette);
    // setMask(pix.mask());
    setAutoFillBackground(true);

    setGeometry(0,0,35,35);

    setWindowFlags(Qt::FramelessWindowHint);

}

tipDia::~tipDia()
{
    delete p;
    delete layout;


}

void tipDia::setPos(QPoint curPoint)
{
    setGeometry(curPoint.x()+30,curPoint.y(),35,35);

}

void tipDia::mousePressEvent(QMouseEvent *)
{

   emit startTranslate();
}


