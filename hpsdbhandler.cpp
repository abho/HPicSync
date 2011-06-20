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
        if(!mQuery->exec("create table if not exists thumbnails (hash varchar(100),dir varchar(100), name varchar(20),lastmodi varchar(20),size int,handyhash varchar(100),"
                         "constraint muh primary key(dir,name))"))
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

void HPSDBHandler::insertHash(const QString &hash, const QString &dir, const QString &name, const QString &lastModi, const int size,const QString &handyHash)
{
    mQuery->prepare("insert into thumbnails(hash,dir,name,lastmodi,size,handyhash)"
                    "values(:hash,:dir,:name,:lastmodi,:size,:handyhash)");
    mQuery->bindValue(":hash",hash);
    mQuery->bindValue(":dir",dir);
    mQuery->bindValue(":name",name);
    mQuery->bindValue(":lastmodi", lastModi);
    mQuery->bindValue(":size", size);
    mQuery->bindValue(":handyhash",handyHash);
    if(! mQuery->exec()){
        //qDebug() << Q_FUNC_INFO <<"insert" <<  mQuery->lastError().text();
    }
}

QList<QStringList> HPSDBHandler::hashPaths(const QString &path)
{
    QList<QStringList> list;
    QStringList hashList,nameList;
    mQuery->prepare("select hash, name from thumbnails where dir = '"+path+"'");
    if(! mQuery->exec()){
        qDebug() << Q_FUNC_INFO <<"select" <<  mQuery->lastError().text();
    }
    while (mQuery->next()) {
        hashList.append(mQuery->value(0).toString());
        nameList.append(mQuery->value(1).toString());

    }
    list.append(hashList);
    list.append(nameList);
    return list;

}
