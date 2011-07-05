#ifndef HPSDIRLISTER_H
#define HPSDIRLISTER_H

#include <QObject>
#include <QtCore>
#include <QDebug>

#include "hpsknotdirmodel.h"

class HPSDirLister : public QObject
{
    Q_OBJECT
public:
    explicit HPSDirLister(HPSKnotDirModel &model, const QString &path,QObject *parent = 0);
    ~HPSDirLister();
    QStringList dirs();

signals:
    void workDone();
    void workSemiDone();
    void chunkDone();
    void dirDone(QString);
public slots:
    void startWork();
    void kill();
private:
    HPSKnotDirModel &mDirModel;
    QString mPath;
    QStringList mList;
    QElapsedTimer timer;
    QDir mDir;
    bool mEx;

    void subDirsFrom(const QString &dir);

};
#endif // HPSAUTHENTICATION_H
