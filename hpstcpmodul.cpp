#include "hpstcpmodul.h"
#include <QDebug>
HPSTCPModul::HPSTCPModul(HPSOption *option,QWidget *parent) :
        QObject(parent),option(option),serverSocket(NULL),ipAddresse("")
{
    this->hParent=parent;
}

void HPSTCPModul::checkResults() {
    this->countFindServer--;
    HPSFindServer *findServer= (HPSFindServer*)sender();
    findServer->disconnect();
    findServer->deleteLater();
    if(this->countFindServer==0){
        if(server.size()>1){
            HPSSelectAddressWidget *selectWidget = new HPSSelectAddressWidget(this->server,this->hParent);
            this->connect(selectWidget,SIGNAL(selected(QTcpSocket*)),this,SLOT(checkSelected(QTcpSocket*)));
            selectWidget->show();
        } else if(server.size()==1) {
            this->startAuth();
        } else {
            QMessageBox::information(NULL,trUtf8("Information"),trUtf8("Es konnte kein Device gefunden werden."),QMessageBox::Ok);
        }
    }
}

void HPSTCPModul::checkSelected(QTcpSocket *socket){
    if(socket != NULL){
        this->serverSocket=socket;
        this->ipAddresse = socket->peerAddress().toString();
        this->startAuth();
    }
    HPSSelectAddressWidget *selectWidget = (HPSSelectAddressWidget*)sender();
    selectWidget->disconnect();
    selectWidget->deleteLater();
    this->server.clear();

}
void HPSTCPModul::startAuth(){
    HPSAuthentication *auth = new HPSAuthentication(option->getUsername(),option->getPassword(),this->serverSocket,this);
    this->connect(auth,SIGNAL(finshed(bool)),this,SLOT(checkAuth(bool)));
    this->connect(auth,SIGNAL(error(QString)),this,SLOT(checkAuthError(QString)));
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
    HPSPasswordWidget *passWidget = new HPSPasswordWidget(this->option,this->hParent);
    passWidget->setLabelText(trUtf8("Folgender Verbindungsfehler trat bei der Authentification auf:\n")+
                             trUtf8("<i>")+errorString+trUtf8("</i>"));
    passWidget->setButtonText(QStringList()<<trUtf8("Neu suchen") << trUtf8("Wiederholen") << trUtf8("Abbrechen"));
    passWidget->show();
}
void HPSTCPModul::checkPassWidget(int choise){
    HPSPasswordWidget *passWidget = (HPSPasswordWidget*)sender();
    switch(choise){
    case 0:
        this->checkUserAndPW(passWidget->getUserAndPassword());
        this->startSearch();
        break;
    case 1:
        this->checkUserAndPW(passWidget->getUserAndPassword());
        this->startAuth();
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
    if(userAndPW.at(0) != this->option->getUsername())
        this->option->setPassword(userAndPW.at(0));
    if(userAndPW.at(1) != this->option->getPassword())
        this->option->setPassword(userAndPW.at(1));
}

void HPSTCPModul::readSocket(){

}

void HPSTCPModul::socketError(QAbstractSocket::SocketError error){
    qDebug() << error;
}

void HPSTCPModul::startSearch(){
    qDebug() << "StartSearch";
    this->countFindServer = 0;
    QList<QHostAddress> list = QNetworkInterface::allAddresses() ;
    QHostAddress address;
    for ( int i = 0;i<list.size();i++){
        address = list.at(i);
        if (address.toString() != "127.0.0.1" && !address.toString().contains(":")){
            qDebug() << "start finder" << address.toString();
            QThread *thread = new QThread();
            HPSFindServer *findServer = new HPSFindServer(server,mutex,address.toString(),this->option->getPort());
            findServer->moveToThread(thread);
            this->connect(thread,SIGNAL(started()),findServer,SLOT(search()));
            this->connect(findServer,SIGNAL(finished()),this,SLOT(checkResults()));
            this->connect(findServer,SIGNAL(destroyed()),thread,SLOT(quit()));
            this->connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
            thread->start();
            this->countFindServer++;
        }
    }
}


