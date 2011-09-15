#ifndef HPSTHREADMANAGER_H
#define HPSTHREADMANAGER_H

#include <QObject>
#include <QtCore>
#include <QtGui>
#include "hpsworkerclass.h"
#include "hpsoption.h"
#include "hpsdbhandler.h"

class HPSImageLoader;
class HPSDirLister;
class HPSKnotDirModel;
class HPSImageLoader;
class HPSDirChecker;
class HPSThreadManager : public QObject
{
    Q_OBJECT
public:
    explicit HPSThreadManager(HPSKnotDirModel &knotModel,HPSOption &option,HPSDBHandler *dbHandler,QObject *parent = 0);
    enum ThreadTyp { DirLister,DirChecker,ImageLoader};


signals:

public slots:
private:
    void initDirLister(HPSKnotDirModel &knotModel);
    void initDirChecker(HPSOption &option,HPSDBHandler *dbHandler);
    void initImagerLoaders();



    QList<QThread> mThreads;
    QVector<HPSImageLoader*> mImageLoaders;
    HPSDirLister *mDirLister;
    HPSDirChecker *mDirChecker;
};

#endif // HPSTHREADMANAGER_H
