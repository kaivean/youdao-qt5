#include <QtGui>
#include "dialog.h"




class App: public QApplication {
public:
    App(int argc, char **argv): QApplication(argc, argv) { }


//#if defined(Q_WS_X11)
//    bool x11EventFilter(XEvent *xe) {

//        switch (xe->type) {
//        case ButtonRelease:

//            //  qDebug() <i< xe->xclent.window << "   " << xe->xclient.data.s;
//            return false;
//        }
//        return false;
//    }
//#endif

};




int main(int argc, char *argv[])
{
    App a(argc, argv);

    Dialog w;
    w.show();

    // str == "Vlad"

  // resultDia curResultDia;
   //curResultDia.show();


    return a.exec();
}
