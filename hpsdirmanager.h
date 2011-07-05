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
class HPSDirManager : public QObject
{
    Q_OBJECT
public:
    explicit HPSDirManager(HPSThumbManager &thumbManager,HPSOption &option,QObject *parent = 0);
    ~HPSDirManager();
    bool startAddDir(const QString  &dir,bool withSub);

    void setModel( QStandardItemModel *model);
    void makeView();
    void removeDir(const QString &dir,bool withSub);
    void saveDirModel();
    void reset();
    const QStringList & dirs();

private:

    void startDirLister(const QString &dir);

    HPSOption &mOption;
    HPSThumbManager &mThumbManager;
    QStandardItemModel *mCurrentModel;
    HPSKnotDirModel mKnotDirModel;
    HPSDomDirModel mDomDirModel;

signals:
    void dirToRemove(QString);
private slots:
    void checkWork();
    void checkDir(QString str);
public slots:
    void finishAddDir(QString str);
};

#endif // HPSDIRMANAGER_H
