#ifndef HPSDBHANDLER_H
#define HPSDBHANDLER_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql>
#include <QDebug>


class HPSDBHandler : public QObject
{
    Q_OBJECT
public:
    explicit HPSDBHandler(QObject *parent = 0);
    bool openDatabase(const QString &name);
    void startTransaction();
    void finishTransaction();
    void removeDir(const QString &dir, bool withSub);
    void removeEntires(const QString &dir,const QStringList &nameList);
    void insertHash(const QString &hash,const QString &dir,const QString &name,const QString &lastModi, const int size,const QString &handyHash);
    void updateInsertHashes(const QString &hash,const QString &dir,const QString &name,const QString &lastModi, const int size,const QString &handyHash);
    QList<QStringList> hashPaths(const QString &path);
    QHash<QString,QString> fotos(const QString &dir);
signals:

public slots:

private:
    QSqlQuery *mQuery;
    QSqlDatabase mDatabase;
};

#endif // HPSDBHANDLER_H
