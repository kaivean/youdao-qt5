#include "resultdia.h"
#include "ui_resultdia.h"
#include "dialog.h"



resultDia::resultDia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::resultDia)
{
    ui->setupUi(this);
    //使Ui可自适应父窗口大小
    //    QVBoxLayout* mainLayout = new QVBoxLayout;
     //   mainLayout->addWidget(ui->verticalLayoutWidget);
   //     setLayout(mainLayout);
   // QLabel *l1=new QLabel(this);
   // l1->setText("<a style='color:red;background-image:url(:/ok.png)'> sdf  </a>");

    //QPushButton *p1 =new QPushButton("AF");
   // p1->setMaximumSize(20,20);
  //  ui->hl1->addWidget(p1); ui->hl1->addWidget(l1);
    ui->tKey->setTextInteractionFlags(Qt::TextSelectableByMouse);
    manager = new QNetworkAccessManager(this);  //新建QNetworkAccessManager对象
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));  //关联信号和槽
   // key= ((Dialog *)parent)->curText;

  //  setAttribute(Qt::WA_DeleteOnClose);
  // setWindowFlags(Qt::WindowStaysOnTopHint);
 //   setWindowFlags(windowFlags() &~ (Qt::WindowMaximizeButtonHint));
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
}

resultDia::~resultDia()
{
    qDebug()<< "delete resultdia";
    delete ui;

}


void resultDia::startTrans(QString key)
{
    curKey=key;
    ui->tKey->setText(key);
    if(key.contains(" ")) {

        qDebug() << "contain keong";
    }
    ui->tTrans->setText("<span style='color:red'>Dowdloading</span>");
    QUrl url("http://fanyi.youdao.com/openapi.do?keyfrom=openyoudao&key=1431965149&type=data&doctype=xml&version=1.1&q="+key.toLower());
    qDebug() << url.toEncoded();
    manager->get(QNetworkRequest(url)); //发送请求


}

void resultDia::getTrans()
{






}

void resultDia::readDict()
{
    reader.readNext();
    QString str=""; //翻译文本

    while (!reader.atEnd()){

        if(reader.isEndElement()){

        }
        if (reader.isStartElement()){

            if(reader.name()=="query"){   //key

                reader.readNext();

                if(reader.isCharacters()) {
                    qDebug()<< reader.text();
                }
            }
            if(reader.name()=="paragraph"){     //有道翻译

                reader.readNext();

                if(reader.isCharacters()) {

                      qDebug()<<"paragraph:  "<< reader.text();  //ps
                      str+=reader.text().toString()+"\n";
//                    QPushButton *p=new QPushButton("");
//                    p->setIcon(QIcon(":/images/sound.png"));
//                    p->setWindowFlags(Qt::FramelessWindowHint);
//                    p->setMaximumSize(25,25);

//                    ui->hl1->addWidget(p);
//                    QString s="<span style='backgound-color'> [%1] </span>";
//                     s=s.arg(reader.text().toString());
//                    QLabel* l=new QLabel(s,this);
//                    ui->hl1->addWidget(l);
//                    ui->hl1->addStretch();
                }

            }

            if(reader.name()=="phonetic"){//音标
                 reader.readNext();
                 if(reader.isCharacters()) {
                     qDebug()<<"音标:" << reader.text();
                 }
             }
            if(reader.name()=="explains"){   // 基本释义



                while (!reader.atEnd()){

                    reader.readNext();
                    if(reader.isEndElement()){
                        if(reader.name()== "explains"){
                            break;
                        }
                    }
                    if (reader.isStartElement()){
                        if(reader.name()=="ex"){
                            reader.readNext();
                            if(reader.isCharacters()) {
                                qDebug()<<" 释义: "<< reader.text(); //trans
                                str += reader.text().toString()+"\n";

                            }
                        }
                    }//if

                }//while结束


            }//基本释义结束

        }//大while结束

        reader.readNext();

    }


      if(str.trimmed().length()== 0 || str.trimmed()== curKey.toLower()){
           ui->tTrans->setText("<span style='color:red'>please click moreButton</span>");

      }
      else  ui->tTrans->setText(str);
   // ui->tTrans->setText( ui->tTrans->text().remove(-1,1));

}

void resultDia::replyFinished(QNetworkReply *reply)
{

   // QTextCodec *codec = QTextCodec::codecForName("utf8");
   // QString all = codec->toUnicode(reply->readAll());

  // qDebug()<<all;
   // reader.setDevice(&file);

    reader.addData(reply->readAll());

    reader.readNext();
    while (!reader.atEnd()){


        if (reader.isStartElement()){

            if(reader.name()=="youdao-fanyi"){

                readDict();



            }
        }
        else{
                reader.readNext();
        }


    }
    reader.clear();


    reply->deleteLater();   //最后要释放reply对象

}


void resultDia::on_pushButton_clicked()
{
        emit RequestMoreTranslate();
}
