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
        qDebug() << Q_FUNC_INFO << "db konnte nicht geöffnet werden";
     return false;
    } else {
        return true;
    }
}
