#include "hpstcpmodul.h"
#include <QDebug>
HPSTCPModul::HPSTCPModul(HPSOption *option,QWidget *parent) :
        QObject(parent),mOption(option),mServerSocket(NULL),mIpAddresse("")
{
    mHParent=parent;
}

void HPSTCPModul::checkResults() {
    mCountFindServer--;
    HPSFindServer *findServer= (HPSFindServer*)sender();
    findServer->disconnect();
    findServer->deleteLater();
    if(mCountFindServer==0){
        if(mServerList.size()>1){
            HPSSelectAddressWidget *selectWidget = new HPSSelectAddressWidget(mServerList,mHParent);
            connect(selectWidget,SIGNAL(selected(QTcpSocket*)),this,SLOT(checkSelected(QTcpSocket*)));
            selectWidget->show();
        } else if(mServerList.size()==1) {
            startAuth();
        } else {
            QMessageBox::information(NULL,trUtf8("Information"),trUtf8("Es konnte kein Device gefunden werden."),QMessageBox::Ok);
        }
    }
}

void HPSTCPModul::checkSelected(QTcpSocket *socket){
    if(socket != NULL){
        mServerSocket=socket;
        mIpAddresse = socket->peerAddress().toString();
        startAuth();
    }
    HPSSelectAddressWidget *selectWidget = (HPSSelectAddressWidget*)sender();
    selectWidget->disconnect();
    selectWidget->deleteLater();
    mServerList.clear();

}
void HPSTCPModul::startAuth(){
    HPSAuthentication *auth = new HPSAuthentication(mOption->getUsername(),mOption->getPassword(),mServerSocket,this);
    connect(auth,SIGNAL(finshed(bool)),this,SLOT(checkAuth(bool)));
    connect(auth,SIGNAL(error(QString)),this,SLOT(checkAuthError(QString)));
}

void HPSTCPModul::checkAuth(bool x) {
    HPSAuthentication *auth =(HPSAuthentication*)sender();
    qDebug() << "auth = " << x;
    if(x){
        auth->disconnect();
        auth->deleteLater();
    } else {

    }
}

void HPSTCPModul::checkAuthError(QString errorString) {
    qDebug() << errorString;
    HPSPasswordWidget *passWidget = new HPSPasswordWidget(mOption,mHParent);
    passWidget->setLabelText(trUtf8("Folgender Verbindungsfehler trat bei der Authentification auf:\n")+
                             trUtf8("<i>")+errorString+trUtf8("</i>"));
    passWidget->setButtonText(QStringList()<<trUtf8("Neu suchen") << trUtf8("Wiederholen") << trUtf8("Abbrechen"));
    passWidget->show();
}
void HPSTCPModul::checkPassWidget(int choise){
    HPSPasswordWidget *passWidget = (HPSPasswordWidget*)sender();
    switch(choise){
    case 0:
        checkUserAndPW(passWidget->getUserAndPassword());
        startSearch();
        break;
    case 1:
        checkUserAndPW(passWidget->getUserAndPassword());
        startAuth();
        break;
    case 2:
        break;
    case -1:
        break;
    }
    passWidget->disconnect();
    passWidget->deleteLater();
}

void HPSTCPModul::checkUserAndPW(const QStringList &userAndPW){
    if(userAndPW.at(0) != mOption->getUsername())
        mOption->setPassword(userAndPW.at(0));
    if(userAndPW.at(1) != mOption->getPassword())
        mOption->setPassword(userAndPW.at(1));
}

void HPSTCPModul::readSocket(){

}

void HPSTCPModul::socketError(QAbstractSocket::SocketError error){
    qDebug() << error;
}

void HPSTCPModul::startSearch(){
    qDebug() << "StartSearch";
    mCountFindServer = 0;
    QList<QHostAddress> list = QNetworkInterface::allAddresses() ;
    QHostAddress address;
    for ( int i = 0;i<list.size();i++){
        address = list.at(i);
        if (address.toString() != "127.0.0.1" && !address.toString().contains(":")){
            qDebug() << "start finder" << address.toString();
            QThread *thread = new QThread();
            HPSFindServer *findServer = new HPSFindServer(mServerList,mMutex,address.toString(),mOption->getPort());
            findServer->moveToThread(thread);
            connect(thread,SIGNAL(started()),findServer,SLOT(search()));
            connect(findServer,SIGNAL(finished()),this,SLOT(checkResults()));
            connect(findServer,SIGNAL(destroyed()),thread,SLOT(quit()));
            connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
            thread->start();
            mCountFindServer++;
        }
    }
}


