#include "hpsoption.h"
#include <QApplication>

HPSOption::HPSOption() :
    setting("hakah","fotoCrapper")
{
    this->quellOrdner =this->setting.value("quellOrdner","").toString();

    if (this->quellOrdner.isEmpty()){
        QDir dir(QApplication::applicationDirPath());
        dir.mkdir("images");
        this->setQuellOrdner(QApplication::applicationDirPath()+"/images");
    }
    this->ordner = this->setting.value("ordner",QStringList() << QApplication::applicationDirPath()+"/images").toStringList();
    this->thumbSize = this->setting.value("thumbSize",100).toInt();
    this->geometry = this->setting.value("geometry",QRect(100,100,300,300)).toRect();
    this->geometryOption = this->setting.value("geoOption",QRect()).toRect();
    this->username = this->setting.value("username","").toString();
    this->password = this->setting.value("password","").toString();
    this->port = this->setting.value("port",1234).toInt();
    this->comboBoxView = this->setting.value("comboBoxView",HPSOption::ListView).toInt();
    comboBoxCurrentDir = this->setting.value("cbCurrentDir","" ).toString();
}


const QString & HPSOption::getQuellOrdner(){
    return this->quellOrdner;
}

int HPSOption::getThumbSize(){
    return this->thumbSize;
}

const QRect & HPSOption::getGeometry(){
    return this->geometry;
}
const QStringList &HPSOption::getOrdner(){
    return this->ordner;
}
void HPSOption::setOrdner(const QStringList &list){
    this->ordner =list;
    setting.setValue("ordner",ordner);
}
void HPSOption::addOrdner(const QString &dir){
    qDebug() << "addordner first" << ordner;
    this->ordner.append(dir);
    setting.setValue("ordner",ordner);
    qDebug() << "addordner" << ordner;
}
const QRect &  HPSOption::getGeometryOption(){
    return this->geometryOption;
}
void HPSOption::setQuellOrdner(const QString &quellOrdner){
    this->quellOrdner = quellOrdner;
    this->setting.setValue("quellOrdner",this->quellOrdner);
}
void HPSOption::setThumbSize(const int size){
    this->thumbSize = size;
    this->setting.setValue("thumbSize",this->thumbSize);
}
void HPSOption::setGeometry(const QRect &geometry){
    this->geometry = geometry;
    this->setting.setValue("geometry",this->geometry);
}

void HPSOption::setGeometryOption(const QRect &geometry){
    this->geometryOption = geometry;
    this->setting.setValue("geoOption",this->geometryOption);
}

const QString & HPSOption::getUsername(){
    return this->username;
}

void HPSOption::setUsername(const QString &username){
    this->username = username;

    this->setting.setValue("username",username);
}

const QString & HPSOption::getPassword(){
    return this->password;
}

void HPSOption::setPassword(const QString &password){
    this->password = password;
    setting.setValue("password",password);
}

int HPSOption::getPort(){
    return this->port;
}

void HPSOption::setPort(int port){
    this->port = port;
    setting.setValue("port",port);
}

int HPSOption::getComboBoxView(){
    return comboBoxView;
}

void  HPSOption::setComboBoxView(int view){
    comboBoxView =view;
    setting.setValue("comboBoxView",comboBoxView);
}

const QString & HPSOption::getComboBoxCurrentDir()
{
    return comboBoxCurrentDir;
}

void HPSOption::setComboBoxCurrentDir(const QString &currentDir)
{
    comboBoxCurrentDir = currentDir;
    setting.setValue("cbCurrentDir",comboBoxCurrentDir);
}

void HPSOption::removeOrdner(const QString &dir)
{
    ordner.removeOne(dir);
    setting.setValue("ordner",ordner);
}
