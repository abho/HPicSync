#include "hpsfindserver.h"
HPSFindServer::HPSFindServer(QList<QTcpSocket *> &list,QMutex &mutex,const QString &ip,const int port, QObject *parent):
        QObject(parent),mFoundedServer(list),mMutex(mutex),mIp(ip),mPort(port)
{
}
HPSFindServer::~HPSFindServer(){
    //qDebug() <<"find server kill";
}
void HPSFindServer::socketError(QAbstractSocket::SocketError error){
    QTcpSocket *s = (QTcpSocket*)sender();
    //qDebug() << error << s;
    delete s;
    mCount--;
    //qDebug() << count;
    if(mCount ==0){
      //  qDebug() << "fertig";
        emit finished();
    }

}
void HPSFindServer::serverFound(){
    QTcpSocket *s =(QTcpSocket*)sender();
    s->disconnect();
    mMutex.lock();
    mFoundedServer.append(s);
    mMutex.unlock();
    //qDebug() << "found server"<<  s << s->peerAddress().toString();
    mCount--;
    //qDebug() << count ;
    if(mCount == 0){
      //  qDebug() << "fertig";
        emit finished();
    }
}

void HPSFindServer::search(){
 //   qDebug() <<"muh"<< ip;
    QString tmpIP= mIp.section(".",0,2)+".";
   // qDebug() << tmpIP;
    mCount = 253;
    for( int i = 1;i<255;i++){
        QTcpSocket *s = new QTcpSocket();
     //   qDebug() << tmpIP+QString::number(i) << s;
        s->connectToHost(tmpIP+QString::number(i),30000);
        connect(s,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socketError(QAbstractSocket::SocketError)));
        connect(s,SIGNAL(connected()),this,SLOT(serverFound()));
    }
}

const QList<QTcpSocket *> & HPSFindServer::result(){
    return mFoundedServer;
}
