#ifndef TIPDIA_H
#define TIPDIA_H

#include <QDialog>
#include <QtCore>
#include <QtGui>

#include <QtNetwork>



class tipDia : public QDialog
{
    Q_OBJECT
public:

    tipDia(QWidget *parent = 0);
    ~tipDia();


     void setPos(QPoint);


     void mousePressEvent(QMouseEvent *);

 public slots:


 private :
     QPushButton * p;
     QHBoxLayout *layout;

signals:
    void startTranslate();
    
public slots:
    
};

#endif // TIPDIA_H
