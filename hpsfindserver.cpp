#include "hpsfindserver.h"
HPSFindServer::HPSFindServer(QList<QTcpSocket *> &list,QMutex &mutex,const QString &ip,const int port, QObject *parent):
        QObject(parent),foundedServer(list),mutex(mutex),ip(ip),port(port)
{
}
HPSFindServer::~HPSFindServer(){
    //qDebug() <<"find server kill";
}
void HPSFindServer::socketError(QAbstractSocket::SocketError error){
    QTcpSocket *s = (QTcpSocket*)sender();
    //qDebug() << error << s;
    delete s;
    this->count--;
    //qDebug() << this->count;
    if(this->count ==0){
      //  qDebug() << "fertig";
        emit this->finished();
    }

}
void HPSFindServer::serverFound(){
    QTcpSocket *s =(QTcpSocket*)sender();
    s->disconnect();
    this->mutex.lock();
    this->foundedServer.append(s);
    this->mutex.unlock();
    //qDebug() << "found server"<<  s << s->peerAddress().toString();
    this->count--;
    //qDebug() << this->count ;
    if(this->count == 0){
      //  qDebug() << "fertig";
        emit this->finished();
    }
}

void HPSFindServer::search(){
 //   qDebug() <<"muh"<< this->ip;
    QString tmpIP= this->ip.section(".",0,2)+".";
   // qDebug() << tmpIP;
    this->count = 253;
    for( int i = 1;i<255;i++){
        QTcpSocket *s = new QTcpSocket();
     //   qDebug() << tmpIP+QString::number(i) << s;
        s->connectToHost(tmpIP+QString::number(i),30000);
        this->connect(s,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socketError(QAbstractSocket::SocketError)));
        this->connect(s,SIGNAL(connected()),this,SLOT(serverFound()));
    }
}

const QList<QTcpSocket *> & HPSFindServer::result(){
    return this->foundedServer;
}
