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
    explicit HPSThumbManager( HPSOption &option, QObject *parent = 0);
    void creatThumbs(const QStringList &cDirs,const bool view);
    void creatThumbs(const QString &cDir,const bool view);

    bool allThreadsClose();
    void closeAllThreads();
    void setDatenBankHandler(HPSDBHandler *handler);
    void setListWidget(QListWidget *listWidget);
    void loadThumbs(const QString & dir);
    bool dirReady(const QString &dir);
    bool startWork();
    int workCount();

signals:
    void allThreadsDestroyed();
    void startThumbCreation(const QString&,const int);
    void thumbsReady(const int);
    void creationReady();
    void startCreation();
public slots:

private:
    HPSOption &mOption;
    HPSDBHandler *mDatabaseHandler;
    QStringList mDirsCheck;
    QString mCurrentDir;
    QVector<HPSThumb> mThumbs;
    QMap<QThread *,HPSWorkerClass *> mThreads;
    QListWidget *mListWidget;
    QElapsedTimer mTimer;
    QMutex mMutex;
    int mCountError;
    int mThumbsLoaded;

    void saveHashes();
    void  reset();
    void  makeThumbsAndView(const QString &cDir,const bool withView);
    void subDirsFrom(const QString &dir,QStringList &dirs);
    void makeView(const QList<QStringList> &list);
    void nextWork();

private slots:
     void fertigTime();
     void threadClear();
     void fotosReady(int pos, int count);
     void fotoReady();
     void checkIfAllClose();
     void getError(int error);
     void hashesReady();

};

#endif // HPSTHUMBMANAGER_H
