#ifndef HPSHASHSAVER_H
#define HPSHASHSAVER_H

#include "hpsworkerclass.h"
#include "hpsdbhandler.h"
#include <QtCore>
#include <QApplication>
#include <QObject>
#include "hpsthumb.h"


class HPSHashSaver : public HPSWorkerClass
{
    Q_OBJECT
public:
    HPSHashSaver( HPSDBHandler *dbHandler,const QVector<HPSThumb> &imageVector,const QString &folder,QObject *parent = 0);
    ~HPSHashSaver();
public slots:
    void start();
signals:
    void finish();
private:
    const QVector<HPSThumb> &mImageVec;
    const QString &mFolder;
    HPSDBHandler *mDatabaseHandler;
};

#endif // HPSHASHSAVER_H
