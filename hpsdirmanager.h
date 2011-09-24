#ifndef HPSDIRMANAGER_H
#define HPSDIRMANAGER_H

#include <QObject>
#include <QStandardItemModel>
#include "hpstreecombobox.h"
#include "hpsoption.h"
#include "hpsdirlister.h"
#include "hpsknotdirmodel.h"
#include "hpsdomdirmodel.h"
#include "hpsthumbmanager.h"
#include "hpsthreadmanager.h"
class HPSDirManager : public QObject
{
    Q_OBJECT
public:
    explicit HPSDirManager(HPSThreadManager &threadManager,HPSDBHandler &dbHandler,HPSOption &option,QObject *parent = 0);
    ~HPSDirManager();
    bool startAddDir(const QString  &dir,bool withSub);

    void setModel( QStandardItemModel *model);
    void makeView();
    void removeDir(const QString &dir,bool withSub);
    void saveDirModel();
    HPSKnotDirModel &knotDirModel();
    void reset();
    QStringList  wholeDirList();
    void setDirHasFiles(const QString &dir, bool hasFiles);
signals:
    void dirToRemove(QString);
    void checkDir(QString dir);
    void startDirLister(QString dir);

public slots:
    void finishAddDir(QString str,bool hasFiles);
private slots:
    void checkWork();
private:

    HPSOption &mOption;
   HPSThreadManager &mThreadManager;
   HPSDBHandler &mDBHandler;
    QStandardItemModel *mCurrentModel;
    HPSKnotDirModel mKnotDirModel;
    HPSDomDirModel mDomDirModel;


};

#endif // HPSDIRMANAGER_H
