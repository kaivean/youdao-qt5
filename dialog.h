#ifndef DIALOG_H
#define DIALOG_H

#include "tipdia.h"
#include "resultdia.h"

#include <QtWidgets>
#include <QtWebKitWidgets/QWebPage>
#include <QtWebKitWidgets/QWebFrame>
#include <QtWebKit/QWebElement>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();


    void closeEvent(QCloseEvent *);

    QClipboard *clip;
    resultDia *curResultDia;
    int MouseState;  //指示鼠标状态
    int IsSeclecting; //是否有过选择行为

    QString preKey; //上个选择的文本
    QString curKey;  //当前选择的文本
    tipDia *curTipDia;  //点击翻译按钮
    QProcess *process;  //启动监听鼠标事件的程序

    QTimer *readTimer;

    QTime time;
    int curTime;


    QWebPage *page;
    QWebPage *prePage;

    void openTranMode();


    int isQueryByCopy; // 0 划词查询   1 复制查询


public slots:
    void HandleSelection();
    void  HandleData();
    void redFromStdOut();
    void startTranslate();


    void startMoreTranslate();
    
private slots:
    void on_pushButton_clicked();


    void pageLoadfinished();

    void on_rMode_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
