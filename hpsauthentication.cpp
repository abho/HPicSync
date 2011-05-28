#include "hpsauthentication.h"
#include <QDebug>
HPSAuthentication::HPSAuthentication(const QString &usernameString, const QString &passwordString,QTcpSocket *socket,QObject *parent) :
        QObject(parent),socket(socket),username(usernameString.toUtf8()),password(passwordString.toUtf8()),
        CHPVERSION("cHamsterProtocolV1.0"),counter(0)
{

    this->hamsterSec1 = QByteArray(9,0);
    this->hamsterSec1[0]=2;
    this->hamsterSec1[1]=7;
    this->hamsterSec1[2]=1;
    this->hamsterSec1[3]=8;
    this->hamsterSec1[4]=2;
    this->hamsterSec1[5]=8;
    this->hamsterSec1[6]=1;
    this->hamsterSec1[7]=8;
    this->hamsterSec1[8]=3;

    this->hamsterSec2 = QByteArray(9,0);
    this->hamsterSec2[0]=3;
    this->hamsterSec2[1]=1;
    this->hamsterSec2[2]=4;
    this->hamsterSec2[3]=1;
    this->hamsterSec2[4]=5;
    this->hamsterSec2[5]=9;
    this->hamsterSec2[6]=2;
    this->hamsterSec2[7]=6;
    this->hamsterSec2[8]=5;
    this->connect(this->socket,SIGNAL(readyRead()),this,SLOT(readNextStringLine()));
    this->connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socketError(QAbstractSocket::SocketError)));
    this->connect(socket,SIGNAL(disconnected()),this,SLOT(socketDisc()));
}

void HPSAuthentication::socketDisc() {
    qDebug() << "socket disconnect";

}

QTcpSocket* HPSAuthentication::getSocket(){
    return this->socket;
}

void HPSAuthentication::socketError(QAbstractSocket::SocketError error){
    qDebug() << error;
    emit this->error(this->socket->errorString());
}

void HPSAuthentication::start() {
    this->socket->write(this->makeHelloResponse());
    this->socket->flush();
    this->currentState = this->ChallangeResponse;
}
int HPSAuthentication::getCounter(){
    return this->counter;
}

QByteArray HPSAuthentication::makeHelloResponse(){

    QByteArray rnonce;
    rnonce.append(QByteArray::number(this->getPositivRandomIntIn(10, 10000)));
    rnonce.append(":");
    rnonce.append(QByteArray::number(this->getCurrentUnixTime()));

    //qDebug() << rnonce << rnonce.size();
    this->rnonceB64 = this->base64(rnonce);
    qDebug() << rnonceB64 << base64Decode(rnonceB64).size();
    QByteArray helloResponse;
    helloResponse.append(this->CHPVERSION);
    helloResponse.append(this->base64(this->aXorB(this->username, base64Decode(this->rnonceB64))));
    helloResponse.append(":");
    helloResponse.append(this->rnonceB64);
    qDebug() << helloResponse;
    return helloResponse+ "\n";
}
void HPSAuthentication::restart(){
    this->counter++;
    this->start();
}

QByteArray HPSAuthentication::makeAuthResponse(const QByteArray &cnonce){
    // Prepare response for auth check
    // Calculate it first
    // response:='B64[MD5[HamsterSec1 XOR (USERNAME XOR MD5(CONCE XOR
    // PASSWORD)]]'
    qDebug() <<"PW:" << this->password.length() << cnonce.length();
    QByteArray passwordXORcnonce = aXorB(this->password, cnonce);
    qDebug()  << "B64 passwordXORcnonce:" << base64(passwordXORcnonce) << " Length:" << passwordXORcnonce.length();

    QByteArray passwordXORcnonceMD5 = this->getMD5Sum(passwordXORcnonce);
    qDebug() << "B64 passwordXORcnonceMD5:"<< base64(passwordXORcnonceMD5) << " Length:"<< passwordXORcnonceMD5.length();

    QByteArray passwordXORcnonceMD5XORusername = aXorB(passwordXORcnonceMD5,username);
    qDebug() << "B64 passwordXORcnonceMD5XORusername:"<< base64(passwordXORcnonceMD5XORusername) + " Length:"
            << passwordXORcnonceMD5XORusername.length();

    QByteArray passwordXORcnonceMD5XORusernameXORhamsterSec1 = aXorB(
            passwordXORcnonceMD5XORusername, this->hamsterSec1);
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
    switch(this->currentState) {
    case ChallangeResponse:
        this->checkChallangeResponse(this->socket->readLine());
        break;
    case AuthRespons:
        this->checkAuthResponse(this->socket->readLine());
        break;
    }
    /*  QString tmpString = "";
        qDebug() <<  this->socket->readLine() << this->socket->bytesAvailable();
    */
}
void HPSAuthentication::checkChallangeResponse(QByteArray block){
    QByteArray challenge = base64Decode(block.replace("\n",""));
    qDebug() << "rnonce" << this->rnonceB64;
    qDebug() <<  "Challenge received:" << block << " Length:"<< challenge.length();

    // Calculate CNONCE

    QByteArray cnonce = aXorB(aXorB(challenge, this->hamsterSec2),
                              base64Decode(this->rnonceB64));

    qDebug() << "CNONCE calculated, should be:" << base64(cnonce);


    this->socket->write(this->makeAuthResponse(cnonce));
    this->socket->flush();
    this->currentState = this->AuthRespons;

}
void HPSAuthentication::checkAuthResponse(QByteArray block){
    this->socket->disconnect();
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
