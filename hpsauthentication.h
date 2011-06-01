#ifndef HPSAUTHENTICATION_H
#define HPSAUTHENTICATION_H

#include <QObject>
#include <QtNetwork>

class HPSAuthentication : public QObject
{
    Q_OBJECT
public:
    explicit HPSAuthentication(const QString &usernameString, const QString &passwordString,QTcpSocket *mSocket,QObject *parent = 0);
    QByteArray makeHelloResponse();
    QByteArray makeAuthResponse(const QByteArray &cnonce);
    int getCounter();
    void start();
    void restart();
    QTcpSocket* getSocket();
private:
    QTcpSocket *mSocket;
    QByteArray mUsername,mPassword,mHamsterSec1,nHamsterSec2,mChpVersion,mRnonceB64;
    enum state {ChallangeResponse, AuthRespons,AcceptResponse};
    state mCurrentState;
    int mCounter;

    int getPositivRandomIntIn(int intervalBegin, int intervalEnd);
    int getCurrentUnixTime();
    QByteArray base64(const QByteArray &byte);
    QByteArray base64Decode(const QByteArray &BASE64String);
    QByteArray aXorB(const QByteArray &aByte, const QByteArray &bByte);
    void checkChallangeResponse(QByteArray block);
    void checkAuthResponse(QByteArray block);
    void checkAcceptResponse(QByteArray block);
    QByteArray getMD5Sum(const QByteArray &block);
signals:
    void finshed(bool);
    void error(QString);

private slots:
    void readNextStringLine();
    void socketError(QAbstractSocket::SocketError err);
    void socketDisc();
};

#endif // HPSAUTHENTICATION_H
