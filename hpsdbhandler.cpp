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
        QSqlQuery query;
        if(!query.exec("create table if not exists thumbnails (hash varchar(100) primary key,dir varchar(100), name varchar(20),handyhash varchar(100))"))
                qDebug() << Q_FUNC_INFO <<"create table" <<  query.lastError().text();
        return true;
    }
}
