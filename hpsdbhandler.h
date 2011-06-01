#ifndef HPSDBHANDLER_H
#define HPSDBHANDLER_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QDebug>


class HPSDBHandler : public QObject
{
    Q_OBJECT
public:
    explicit HPSDBHandler(QObject *parent = 0);
    bool openDatabase(const QString &name);
signals:

public slots:

private:
    QSqlDatabase mDatabase;
};

#endif // HPSDBHANDLER_H
