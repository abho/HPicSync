#include "hpsoption.h"
#include <QApplication>

HPSOption::HPSOption() :
    mSetting("hakah","fotoCrapper")
{
    mQuellOrdner =mSetting.value("quellOrdner","").toString();

    if (mQuellOrdner.isEmpty()){
        QDir dir(QApplication::applicationDirPath());
        dir.mkdir("images");
        setQuellOrdner(QApplication::applicationDirPath()+"/images");
    }
    mOrdner = mSetting.value("ordner",QStringList() << QApplication::applicationDirPath()+"/images").toStringList();
    mThumbSize = mSetting.value("thumbSize",100).toInt();
    mGeometry = mSetting.value("geometry",QRect(100,100,300,300)).toRect();
    mGeometryOption = mSetting.value("geoOption",QRect()).toRect();
    mUsername = mSetting.value("username","").toString();
    mPassword = mSetting.value("password","").toString();
    mPort = mSetting.value("port",1234).toInt();
    mComboBoxView = mSetting.value("comboBoxView",HPSOption::ListView).toInt();
    mComboBoxCurrentDir = mSetting.value("cbCurrentDir","" ).toString();
    mExpandDirs = mSetting.value("expandDirs",QStringList()).toStringList();
}


const QString & HPSOption::getQuellOrdner(){
    return mQuellOrdner;
}

int HPSOption::getThumbSize(){
    return mThumbSize;
}

const QRect & HPSOption::getGeometry(){
    return mGeometry;
}
const QStringList &HPSOption::getOrdner(){
    return mOrdner;
}
void HPSOption::setOrdner(const QStringList &list){
    mOrdner =list;
    mSetting.setValue("ordner",mOrdner);
}
void HPSOption::addOrdner(const QString &dir){
    qDebug() << "addordner first" << mOrdner;
    mOrdner.append(dir);
    mSetting.setValue("ordner",mOrdner);
    qDebug() << "addordner" << mOrdner;
}
const QRect &  HPSOption::getGeometryOption(){
    return mGeometryOption;
}
void HPSOption::setQuellOrdner(const QString &quellOrdner){
    mQuellOrdner = quellOrdner;
    mSetting.setValue("quellOrdner",mQuellOrdner);
}
void HPSOption::setThumbSize(const int size){
    mThumbSize = size;
    mSetting.setValue("thumbSize",mThumbSize);
}
void HPSOption::setGeometry(const QRect &geometry){
    mGeometry = geometry;
    mSetting.setValue("geometry",mGeometry);
}

void HPSOption::setGeometryOption(const QRect &geometry){
    mGeometryOption = geometry;
    mSetting.setValue("geoOption",mGeometryOption);
}

const QString & HPSOption::getUsername(){
    return mUsername;
}

void HPSOption::setUsername(const QString &username){
    mUsername = username;

    mSetting.setValue("username",username);
}

const QString & HPSOption::getPassword(){
    return mPassword;
}

void HPSOption::setPassword(const QString &password){
    mPassword = password;
    mSetting.setValue("password",password);
}

int HPSOption::getPort(){
    return mPort;
}

void HPSOption::setPort(int port){
    mPort = port;
    mSetting.setValue("port",port);
}

int HPSOption::getComboBoxView(){
    return mComboBoxView;
}

void  HPSOption::setComboBoxView(int view){
    mComboBoxView =view;
    mSetting.setValue("comboBoxView",mComboBoxView);
}

const QString & HPSOption::getComboBoxCurrentDir()
{
    return mComboBoxCurrentDir;
}

void HPSOption::setComboBoxCurrentDir(const QString &currentDir)
{
    mComboBoxCurrentDir = currentDir;
    mSetting.setValue("cbCurrentDir",mComboBoxCurrentDir);
}

void HPSOption::removeOrdner(const QString &dir)
{
    mOrdner.removeOne(dir);
    mSetting.setValue("ordner",mOrdner);
}

const QStringList & HPSOption::expandDirs()
{
    return  mExpandDirs;
}

void HPSOption::setExpandDirs(const QStringList &dirs)
{
    mExpandDirs = dirs;
    mSetting.setValue("expandDirs", mExpandDirs);
}
