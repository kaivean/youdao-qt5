#ifndef RESULTDIA_H
#define RESULTDIA_H


#include <QtGui>
#include <QtWidgets>
#include <QtNetwork>

namespace Ui {
class resultDia;
}

class resultDia : public QDialog
{
    Q_OBJECT
    
public:
    explicit resultDia(QWidget *parent = 0);
    ~resultDia();

    QString curKey;

    QNetworkAccessManager *manager;
    QXmlStreamReader  reader;

    void startTrans(QString);

    void getTrans();





private:
    Ui::resultDia *ui;

    void readDict();

signals:
    void RequestMoreTranslate();

private slots:
    void replyFinished(QNetworkReply * reply);
    void on_pushButton_clicked();
};

#endif // RESULTDIA_H
