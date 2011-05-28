#ifndef HPSTCPMODUL_H
#define HPSTCPMODUL_H

#include <QObject>
#include "hpsfindserver.h"
#include "hpsauthentication.h"
#include "hpsoption.h"
#include "hpsselectaddresswidget.h"
#include "hpspasswordwidget.h"
#include <QHash>
#include <QtNetwork>
class HPSTCPModul : public QObject
{
    Q_OBJECT
public:
    explicit HPSTCPModul(HPSOption *option,QWidget *parent = 0);
    void startSearch();
private:
    HPSOption *option;
    QList<QTcpSocket *> server;
    QWidget *hParent;
    QMutex mutex;
    QTcpSocket *serverSocket;
    QString ipAddresse;
    int countFindServer;
    void startAuth();
    void checkUserAndPW(const QStringList &UserAndPW);
private slots:
    void checkResults();
    void checkAuth(bool x);
    void checkAuthError(QString errorString);
    void readSocket();
    void socketError(QAbstractSocket::SocketError error);
    void checkSelected(QTcpSocket *socket);
    void checkPassWidget(int);

};

#endif // HPSTCPMODUL_H
