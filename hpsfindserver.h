#ifndef HPSFINDSERVER_H
#define HPSFINDSERVER_H

#include <QtCore>
#include <QtNetwork>

class HPSFindServer :public QObject
{
    Q_OBJECT
public:
    HPSFindServer( QList<QTcpSocket *> &list,QMutex &mutex, const QString & ip,const int port , QObject *parent = 0);
    ~HPSFindServer();
    const QList<QTcpSocket *> & result();
private:
    QList<QTcpSocket *> &mFoundedServer;
    QMutex &mMutex;
    QString mIp;
    const int mPort;
    int mCount;

public slots:
    void search();
private slots:
    void serverFound();
    void socketError(QAbstractSocket::SocketError error);
signals:
    void  finished();
};

#endif // HPSFINDSERVER_H
