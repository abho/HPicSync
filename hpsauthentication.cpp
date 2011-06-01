#include "hpsauthentication.h"
#include <QDebug>
HPSAuthentication::HPSAuthentication(const QString &usernameString, const QString &passwordString,QTcpSocket *socket,QObject *parent) :
        QObject(parent),mSocket(socket),mUsername(usernameString.toUtf8()),mPassword(passwordString.toUtf8()),
        mChpVersion("cHamsterProtocolV1.0"),mCounter(0)
{

    mHamsterSec1 = QByteArray(9,0);
    mHamsterSec1[0]=2;
    mHamsterSec1[1]=7;
    mHamsterSec1[2]=1;
    mHamsterSec1[3]=8;
    mHamsterSec1[4]=2;
    mHamsterSec1[5]=8;
    mHamsterSec1[6]=1;
    mHamsterSec1[7]=8;
    mHamsterSec1[8]=3;

    nHamsterSec2 = QByteArray(9,0);
    nHamsterSec2[0]=3;
    nHamsterSec2[1]=1;
    nHamsterSec2[2]=4;
    nHamsterSec2[3]=1;
    nHamsterSec2[4]=5;
    nHamsterSec2[5]=9;
    nHamsterSec2[6]=2;
    nHamsterSec2[7]=6;
    nHamsterSec2[8]=5;
    connect(mSocket,SIGNAL(readyRead()),this,SLOT(readNextStringLine()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socketError(QAbstractSocket::SocketError)));
    connect(socket,SIGNAL(disconnected()),this,SLOT(socketDisc()));
}

void HPSAuthentication::socketDisc() {
    qDebug() << "socket disconnect";

}

QTcpSocket* HPSAuthentication::getSocket(){
    return mSocket;
}

void HPSAuthentication::socketError(QAbstractSocket::SocketError err){
    qDebug() << err;
    emit error(mSocket->errorString());
}

void HPSAuthentication::start() {
    mSocket->write(makeHelloResponse());
    mSocket->flush();
    mCurrentState = ChallangeResponse;
}
int HPSAuthentication::getCounter(){
    return mCounter;
}

QByteArray HPSAuthentication::makeHelloResponse(){

    QByteArray rnonce;
    rnonce.append(QByteArray::number(getPositivRandomIntIn(10, 10000)));
    rnonce.append(":");
    rnonce.append(QByteArray::number(getCurrentUnixTime()));

    //qDebug() << rnonce << rnonce.size();
    mRnonceB64 = base64(rnonce);
    qDebug() << mRnonceB64 << base64Decode(mRnonceB64).size();
    QByteArray helloResponse;
    helloResponse.append(mChpVersion);
    helloResponse.append(base64(aXorB(mUsername, base64Decode(mRnonceB64))));
    helloResponse.append(":");
    helloResponse.append(mRnonceB64);
    qDebug() << helloResponse;
    return helloResponse+ "\n";
}
void HPSAuthentication::restart(){
    mCounter++;
    start();
}

QByteArray HPSAuthentication::makeAuthResponse(const QByteArray &cnonce){
    // Prepare response for auth check
    // Calculate it first
    // response:='B64[MD5[HamsterSec1 XOR (USERNAME XOR MD5(CONCE XOR
    // PASSWORD)]]'
    qDebug() <<"PW:" << mPassword.length() << cnonce.length();
    QByteArray passwordXORcnonce = aXorB(mPassword, cnonce);
    qDebug()  << "B64 passwordXORcnonce:" << base64(passwordXORcnonce) << " Length:" << passwordXORcnonce.length();

    QByteArray passwordXORcnonceMD5 = getMD5Sum(passwordXORcnonce);
    qDebug() << "B64 passwordXORcnonceMD5:"<< base64(passwordXORcnonceMD5) << " Length:"<< passwordXORcnonceMD5.length();

    QByteArray passwordXORcnonceMD5XORusername = aXorB(passwordXORcnonceMD5,mUsername);
    qDebug() << "B64 passwordXORcnonceMD5XORusername:"<< base64(passwordXORcnonceMD5XORusername) + " Length:"
            << passwordXORcnonceMD5XORusername.length();

    QByteArray passwordXORcnonceMD5XORusernameXORhamsterSec1 = aXorB(
            passwordXORcnonceMD5XORusername, mHamsterSec1);
    qDebug() << "B64 passwordXORcnonceMD5XORusernameXORhamsterSec1:"
            << base64(passwordXORcnonceMD5XORusernameXORhamsterSec1)
            << " Length:"
            << passwordXORcnonceMD5XORusernameXORhamsterSec1.length();
    // The whole response
    QByteArray response = base64(getMD5Sum(passwordXORcnonceMD5XORusernameXORhamsterSec1));
    qDebug()<< "Response calculated, sending:" << response;
    return response+"\n";
}

void HPSAuthentication::readNextStringLine() {
    switch(mCurrentState) {
    case ChallangeResponse:
        checkChallangeResponse(mSocket->readLine());
        break;
    case AuthRespons:
        checkAuthResponse(mSocket->readLine());
        break;
    }
    /*  QString tmpString = "";
        qDebug() <<  ->socket->readLine() << ->socket->bytesAvailable();
    */
}
void HPSAuthentication::checkChallangeResponse(QByteArray block){
    QByteArray challenge = base64Decode(block.replace("\n",""));
    qDebug() << "rnonce" << mRnonceB64;
    qDebug() <<  "Challenge received:" << block << " Length:"<< challenge.length();

    // Calculate CNONCE

    QByteArray cnonce = aXorB(aXorB(challenge, nHamsterSec2),
                              base64Decode(mRnonceB64));

    qDebug() << "CNONCE calculated, should be:" << base64(cnonce);


    mSocket->write(makeAuthResponse(cnonce));
    mSocket->flush();
    mCurrentState = AuthRespons;

}
void HPSAuthentication::checkAuthResponse(QByteArray block){
    mSocket->disconnect();
    if (block.contains("SUCCESS"))
        emit finshed(true);
    else
        emit finshed(false);

}

int HPSAuthentication::getPositivRandomIntIn(int intervalBegin, int intervalEnd) {
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    int nr=((qrand() % (intervalEnd+1) + intervalBegin));
    if(nr>intervalEnd){
        nr =intervalEnd;
    }
    return nr;
}
int HPSAuthentication::getCurrentUnixTime() {
    return (int) (QDateTime::currentMSecsSinceEpoch()/1000L);
}


QByteArray HPSAuthentication::base64(const QByteArray &byte) {
    return byte.toBase64();
}
QByteArray HPSAuthentication::base64Decode(const QByteArray &BASE64String) {
    return QByteArray::fromBase64(BASE64String);
}

QByteArray HPSAuthentication::aXorB(const QByteArray &aByte, const QByteArray &bByte) {
    // For size correction
    QByteArray cByte(aByte.length(),0);

    // Setting correct size of b
    if (aByte.length() > bByte.length()) {
        for (int i = 0; i < cByte.length(); i++) {
            if (i < bByte.length())
                cByte[i] = bByte[i];
            else
                cByte[i] = bByte[i % bByte.length()];
        }
    } else if (aByte.length() < bByte.length())
        for (int i = 0; i < aByte.length(); i++)
            cByte[i] = bByte[i];
    else
        cByte = bByte;

    QByteArray resultByte(aByte.length(),0);
    for (int i = 0; i < aByte.length(); i++) {

        resultByte[i] =  (aByte.at(i) ^ cByte.at(i));
    }
    return resultByte;
}
QByteArray HPSAuthentication::getMD5Sum(const QByteArray &block){
    return QCryptographicHash::hash(block,QCryptographicHash::Md5);
}
