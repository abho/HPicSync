#ifndef HPSDIRWATCHER_H
#define HPSDIRWATCHER_H

#include <QObject>
#include <QDebug>
#include <QtCore>
#include "hpsdbhandler.h"
#include "hpsdirmanager.h"
#include "hpsoption.h"
#include "hpsthreadmanager.h"
#include "hpsimageloader.h"

class HPSDirWatcher : public QObject
{
    Q_OBJECT
public:
    explicit HPSDirWatcher(HPSThreadManager &threadManager,HPSOption &option,HPSDirManager &dirManager, HPSDBHandler &dbhandler,QObject *parent = 0);
    ~HPSDirWatcher();
    void close();
    void setNextDir(const QString &dir);
signals:
    void startImageloader();
    void updateCurrent();
public slots:
    void startFirstRun();
    void popupWatcher(QStringList list);

    void imageLoaderFertig();
    void imageLoaderError(int errorPos);
    void onImageLoaderDestroyed();
    void setWatcherOn(const QString &dir);
    void stopWatching();
private slots:
    void timerCheckDir();

private:
    HPSDBHandler &mDBHandler;
    HPSOption &mOption;
    HPSDirManager &mDirManager;
    QHash<QString,QFileInfoList > mQueue;
    QVector<HPSThumb> mThumbs;
    HPSImageLoader *mImageLoader;
    QString mNextDir;
    QString mCurrentWatched;
    QTimer mTimer;
    bool mShutDown;
    bool mIsRunning;
    bool mIsImageLoaderRunning;
    bool mIsWatching;
    QFileSystemWatcher mWatcher;

    void checkDir(const QString &dir);
    void prepareImageLoader(const QString &dir,const QFileInfoList &infoList);
    void nextTask();
    void saveToDB( const QString &dir);
};

#endif // HPSDIRWATCHER_H
