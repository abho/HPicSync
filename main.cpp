#include <QtGui/QApplication>
#include"hpicsync.h"

int main(int argc,char *argv[]){
    QApplication a(argc,argv);
   HPicSync q;
   q.show();
   return a.exec();
}
