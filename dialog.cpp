#include "dialog.h"
#include "ui_dialog.h"


#include <QtWidgets>





Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);

    MouseState=0;
    IsSeclecting=0;
    isQueryByCopy=0;
    curResultDia=NULL;


    curTipDia =new tipDia();
    connect(curTipDia,SIGNAL(startTranslate()),this,SLOT(startTranslate()));

    clip=QApplication::clipboard();
    process=NULL;


   // ui->webView->hide();


    page=  ui->webView->page();
    connect(page,SIGNAL(loadFinished(bool)),this,SLOT(pageLoadfinished()));

    QWebSettings *settings = QWebSettings::globalSettings();
    settings->setAttribute(QWebSettings::PluginsEnabled, true);
    settings->setAttribute(QWebSettings::JavascriptEnabled, true);

    prePage=new QWebPage();
    QWebFrame* f=prePage->mainFrame();
    connect(f,SIGNAL(loadFinished(bool)),this,SLOT(pageLoadfinished()));

    openTranMode();

}

Dialog::~Dialog()
{

}

void Dialog::closeEvent(QCloseEvent *)
{


    if(curResultDia) delete curResultDia;

    if(curTipDia) delete curTipDia;
    delete ui;

    if(process){

        process->close();
        delete process;
    }
}

void Dialog::openTranMode()
{
    if(!isQueryByCopy){  //如果不使用复制翻译
        disconnect(clip,SIGNAL(dataChanged()),this,SLOT(HandleData()));

        connect(clip,SIGNAL(selectionChanged()),this,SLOT(HandleSelection()));
        process=new QProcess(this);
        QString file=QCoreApplication::applicationDirPath ();//获取当前应用程序路径

        file=file +"/record/record";
        qDebug() << file;

        process->setProcessChannelMode(QProcess::MergedChannels);
        connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(redFromStdOut()));
        process->start(file);
        time.start();
    }
    else{
          disconnect(clip,SIGNAL(selectionChanged()),this,SLOT(HandleSelection()));
          if (process){
               process->kill();
               delete process;
          }
          process=NULL;
          connect(clip,SIGNAL(dataChanged()),this,SLOT(HandleData()));
    }

}



void Dialog::startTranslate()
{

      curTipDia->hide();
      curKey= curKey.trimmed();

      if(curKey.length()>50){
          qDebug()<<"curkey  :"<< curKey.length();
        return ;
      }
      if(curResultDia){
        //    curResultDia->key=curText;
          qDebug("old resultdia");
        //  curResultDia->hide();
          if(curResultDia->isMinimized()){
               qDebug()<< "showmini";
               curResultDia->showMaximized();
          }
          if(curResultDia->isHidden())
          {    curResultDia->show();
              qDebug()<< "show";}
       qDebug() <<curResultDia->statusTip();
       // curResultDia->activateWindow();
        //  activateWindow();
       //  curResultDia->setWindowFlags(Qt::WindowStaysOnTopHint);

          curResultDia->startTrans(curKey);


      }
      else{
          curResultDia=new resultDia();
          connect(curResultDia,SIGNAL(RequestMoreTranslate()),this,SLOT(startMoreTranslate()));
          qDebug("new resultdia");
          curResultDia->show();
          curResultDia->startTrans(curKey);
      }


}

void Dialog::startMoreTranslate()
{

    show();
    activateWindow();
    ui->tSearchKey->setText(curKey);
    QString url="http://dict.baidu.com/s?wd=" +curKey;
    QUrl u(url);
   // ui->webView->load(u);

    qDebug("staert");
    QWebFrame* f=prePage->mainFrame();


    f->load(url);

}


void Dialog::HandleSelection()  //用于选择模式   当selection改变时调用   与redFromStdOut()一起实现划词功能
{

        qDebug()<< time.elapsed();
        IsSeclecting=1;
        if(MouseState and ((time.elapsed()-curTime)<=2)) { //解决bug 即: 文本框输入文本时激活了划词功能 从而弹出提示框 ，影响输入
            preKey=curKey;
            curKey=  clip->text(QClipboard::Selection);
            QPoint curPoint;
            curPoint = QCursor::pos();//获取当前光标的位置

            curTipDia->show();
            curTipDia->setPos(curPoint);
            IsSeclecting=0;
            qDebug() <<curKey << "selection" ;
        }

}

void Dialog::HandleData()    //用于复制模式
{
     curKey=  clip->text(QClipboard::Selection);
     startTranslate();


}

void Dialog::redFromStdOut() //当record进程每次返回鼠标状态时调用
{

     // qDebug() << "mousedown";
     QString d=QString(process->readAllStandardOutput());
     if(d=="0") { //此时鼠标按下
         //qDebug() << "mousedown";

         IsSeclecting=0; //鼠标按下时 并没有选择文本   当鼠标松开时才察看是否有选择文本
         MouseState=0;

     }
     if(d=="1") {   //此时鼠标松开
         time.restart();
         curTime=time.elapsed();
         qDebug() << curTime;
         MouseState=1;
         if(!curTipDia->isHidden()){
             curTipDia->hide();
         }
         if(IsSeclecting){ //当selection改变 并且鼠标松开时  即可获取selection文本 实现划词
             preKey=curKey;
             curKey=  clip->text(QClipboard::Selection);
             QPoint curPoint;
             curPoint = QCursor::pos();//获取当前光标的位置

             curTipDia->show();   //使提示框位于选择文本附近
             curTipDia->setPos(curPoint);

             qDebug() <<curKey ;

         }
         IsSeclecting=0;//处理完后置0

     }


}

void Dialog::on_pushButton_clicked()
{
      curKey=  clip->text(QClipboard::Selection);
      qDebug()<< curKey;
      curTipDia->activateWindow();
      qDebug() << curTipDia->isActiveWindow()<<"  "<<curTipDia->isVisible();

}


void Dialog::pageLoadfinished()//

{

    qDebug()<< "finish";

  //  QFile file("a.js");
 //   if(!file.open(QFile::ReadOnly)){

  //  }
  //  QString jsStr=QString::fromUtf8(file.readAll());
    // ui->webView->hide();
     QWebFrame *frame=prePage->mainFrame();
   //  frame->evaluateJavaScript(jsStr);
   //  qDebug()<< frame->contentsSize();

     QWebElement doc = frame->documentElement();
     QWebElement head = doc.findFirst("#head");
     head.removeFromDocument();


     QWebElement headBar = doc.findFirst("#head-bar");
     headBar.removeFromDocument();


     QWebElement foot = doc.findFirst("#foot");
     foot.removeFromDocument();

     QWebElement container= doc.findFirst("#container");
     container.setStyleProperty("width","850px");
     ui->webView->setPage(prePage);///不直接使用webview的page对象    先处理page再显示

}





void Dialog::on_rMode_clicked()
{
    if( ui->rMode->isChecked())   isQueryByCopy=1;
    else isQueryByCopy=0;
    openTranMode();
}
