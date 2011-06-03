#ifndef HPSTHUMBMANAGER_H
#define HPSTHUMBMANAGER_H

#include <QObject>
#include "hpsimageloader.h"
#include "hpsdbhandler.h"
#include <QListWidget>
#include <QMessageBox>
#include <QMainWindow>
#include <QApplication>
#include "hpsworkerclass.h"
#include "hpshashsaver.h"
#include "hpsthumb.h"
#include "hpsoption.h"

class HPSThumbManager : public QObject
{
    Q_OBJECT
public:
    explicit HPSThumbManager( HPSOption &option, Object *parent = 0);
    int creatThumbsAndView(const int thumbSize,const QString &cDir,QListWidget *listWidgte);
    int creatThumbs(const int thumbSize,const QString &cDir);
    bool allThreadsClose();
    void closeAllThreads();
    void setDatenBankHandler(HPSDBHandler *handler);
    bool dirReady(const QString &dir);
signals:
    void thumsLoaded(int);
    void allThreadsDestroyed();
public slots:

private:
    HPSOption &mOption;
    HPSDBHandler *mDatabaseHandler;
    QStringList mDirsCheck;
    QStringList mDirsQueue;
    QString mCurrentDir;
    QVector<HPSThumb> mThumbs;    
    QMap<QThread *,HPSWorkerClass *> mThreads;
    QListWidget *mListWidget;
    QElapsedTimer mTimer;
     QMutex mMutex;
    int mCountError;
    int mThumbsLoaded;
    int mCurrentThumbSize;

    void saveHashes();
    void  reset();
    int makeThumbsAndView(const int thumbSize,const QString &cDir,QListWidget *listWidgte);

private slots:
     void fertigTime();
     void threadClear();
     void fotosReady(int pos, int count,const QString &str);
     void checkIfAllClose();
     void getError(int error);
     void hashesReady();

};

#endif // HPSTHUMBMANAGER_H
