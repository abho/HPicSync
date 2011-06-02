#include "hpsdbhandler.h"

HPSDBHandler::HPSDBHandler(QObject *parent) :
    QObject(parent)
{

}

bool HPSDBHandler::openDatabase(const QString &name)
{
    mDatabase = QSqlDatabase::addDatabase("QSQLITE");
    mDatabase.setDatabaseName(name);
    if (!mDatabase.open()) {
        qDebug() << Q_FUNC_INFO << "db konnte nicht geoeffnet werden";
        return false;
    } else {
        mQuery = new QSqlQuery(mDatabase);
        if(!mQuery->exec("create table if not exists thumbnails (hash varchar(100) primary key,dir varchar(100), name varchar(20),handyhash varchar(100))"))
            qDebug() << Q_FUNC_INFO <<"create table" <<  mQuery->lastError().text();
        return true;
    }
}

void HPSDBHandler::startTransaction()
{
    if(mDatabase.isOpen())
        mDatabase.transaction();
}

void HPSDBHandler::finishTransaction()
{
    if(mDatabase.isOpen())
        mDatabase.commit();
}

void HPSDBHandler::insertHash(const QString &hash, const QString &dir, const QString &name, const QString &handyHash)
{
    mQuery->prepare("insert into thumbnails(hash,dir,name,handyhash)"
                 "values(:hash,:dir,:name,:handyhash)");
    mQuery->bindValue(":hash",hash);
    mQuery->bindValue(":dir",dir);
    mQuery->bindValue(":name",name);
    mQuery->bindValue(":handyhash",handyHash);
   if(! mQuery->exec())
       qDebug() << Q_FUNC_INFO <<"insert" <<  mQuery->lastError().text();
}
