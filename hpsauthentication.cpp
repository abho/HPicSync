#include "hpsauthentication.h"
#include <QDebug>
HPSAuthentication::HPSAuthentication(const QString &usernameString, const QString &passwordString,QTcpSocket *socket,QObject *parent) :
        QObject(parent),mSocket(socket),mUsername(usernameString.toUtf8()),mPassword(passwordString.toUtf8()),
        mChpVersion("cHamsterProtocolV1.0"),mCounter(0)
{

    this->mHamsterSec1 = QByteArray(9,0);
    this->mHamsterSec1[0]=2;
    this->mHamsterSec1[1]=7;
    this->mHamsterSec1[2]=1;
    this->mHamsterSec1[3]=8;
    this->mHamsterSec1[4]=2;
    this->mHamsterSec1[5]=8;
    this->mHamsterSec1[6]=1;
    this->mHamsterSec1[7]=8;
    this->mHamsterSec1[8]=3;

    this->nHamsterSec2 = QByteArray(9,0);
    this->nHamsterSec2[0]=3;
    this->nHamsterSec2[1]=1;
    this->nHamsterSec2[2]=4;
    this->nHamsterSec2[3]=1;
    this->nHamsterSec2[4]=5;
    this->nHamsterSec2[5]=9;
    this->nHamsterSec2[6]=2;
    this->nHamsterSec2[7]=6;
    this->nHamsterSec2[8]=5;
    this->connect(this->mSocket,SIGNAL(readyRead()),this,SLOT(readNextStringLine()));
    this->connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socketError(QAbstractSocket::SocketError)));
    this->connect(socket,SIGNAL(disconnected()),this,SLOT(socketDisc()));
}

void HPSAuthentication::socketDisc() {
    qDebug() << "socket disconnect";

}

QTcpSocket* HPSAuthentication::getSocket(){
    return this->mSocket;
}

void HPSAuthentication::socketError(QAbstractSocket::SocketError error){
    qDebug() << error;
    emit this->error(this->mSocket->errorString());
}

void HPSAuthentication::start() {
    this->mSocket->write(this->makeHelloResponse());
    this->mSocket->flush();
    this->mCurrentState = this->ChallangeResponse;
}
int HPSAuthentication::getCounter(){
    return this->mCounter;
}

QByteArray HPSAuthentication::makeHelloResponse(){

    QByteArray rnonce;
    rnonce.append(QByteArray::number(this->getPositivRandomIntIn(10, 10000)));
    rnonce.append(":");
    rnonce.append(QByteArray::number(this->getCurrentUnixTime()));

    //qDebug() << rnonce << rnonce.size();
    this->mRnonceB64 = this->base64(rnonce);
    qDebug() << mRnonceB64 << base64Decode(mRnonceB64).size();
    QByteArray helloResponse;
    helloResponse.append(this->mChpVersion);
    helloResponse.append(this->base64(this->aXorB(this->mUsername, base64Decode(this->mRnonceB64))));
    helloResponse.append(":");
    helloResponse.append(this->mRnonceB64);
    qDebug() << helloResponse;
    return helloResponse+ "\n";
}
void HPSAuthentication::restart(){
    this->mCounter++;
    this->start();
}

QByteArray HPSAuthentication::makeAuthResponse(const QByteArray &cnonce){
    // Prepare response for auth check
    // Calculate it first
    // response:='B64[MD5[HamsterSec1 XOR (USERNAME XOR MD5(CONCE XOR
    // PASSWORD)]]'
    qDebug() <<"PW:" << this->mPassword.length() << cnonce.length();
    QByteArray passwordXORcnonce = aXorB(this->mPassword, cnonce);
    qDebug()  << "B64 passwordXORcnonce:" << base64(passwordXORcnonce) << " Length:" << passwordXORcnonce.length();

    QByteArray passwordXORcnonceMD5 = this->getMD5Sum(passwordXORcnonce);
    qDebug() << "B64 passwordXORcnonceMD5:"<< base64(passwordXORcnonceMD5) << " Length:"<< passwordXORcnonceMD5.length();

    QByteArray passwordXORcnonceMD5XORusername = aXorB(passwordXORcnonceMD5,mUsername);
    qDebug() << "B64 passwordXORcnonceMD5XORusername:"<< base64(passwordXORcnonceMD5XORusername) + " Length:"
            << passwordXORcnonceMD5XORusername.length();

    QByteArray passwordXORcnonceMD5XORusernameXORhamsterSec1 = aXorB(
            passwordXORcnonceMD5XORusername, this->mHamsterSec1);
    qDebug() << "B64 passwordXORcnonceMD5XORusernameXORhamsterSec1:"
            << base64(passwordXORcnonceMD5XORusernameXORhamsterSec1)
            << " Length:"
            << passwordXORcnonceMD5XORusernameXORhamsterSec1.length();
    // The whole response
    QByteArray response = this->base64(this->getMD5Sum(passwordXORcnonceMD5XORusernameXORhamsterSec1));
    qDebug()<< "Response calculated, sending:" << response;
    return response+"\n";
}

void HPSAuthentication::readNextStringLine() {
    switch(this->mCurrentState) {
    case ChallangeResponse:
        this->checkChallangeResponse(this->mSocket->readLine());
        break;
    case AuthRespons:
        this->checkAuthResponse(this->mSocket->readLine());
        break;
    }
    /*  QString tmpString = "";
        qDebug() <<  this->socket->readLine() << this->socket->bytesAvailable();
    */
}
void HPSAuthentication::checkChallangeResponse(QByteArray block){
    QByteArray challenge = base64Decode(block.replace("\n",""));
    qDebug() << "rnonce" << this->mRnonceB64;
    qDebug() <<  "Challenge received:" << block << " Length:"<< challenge.length();

    // Calculate CNONCE

    QByteArray cnonce = aXorB(aXorB(challenge, this->nHamsterSec2),
                              base64Decode(this->mRnonceB64));

    qDebug() << "CNONCE calculated, should be:" << base64(cnonce);


    this->mSocket->write(this->makeAuthResponse(cnonce));
    this->mSocket->flush();
    this->mCurrentState = this->AuthRespons;

}
void HPSAuthentication::checkAuthResponse(QByteArray block){
    this->mSocket->disconnect();
    if (block.contains("SUCCESS"))
        emit this->finshed(true);
    else
        emit this->finshed(false);

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
