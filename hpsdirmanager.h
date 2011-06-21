#ifndef HPSDIRMANAGER_H
#define HPSDIRMANAGER_H

#include <QObject>
#include <QStandardItemModel>
#include "hpstreecombobox.h"
#include "hpsoption.h"
#include "hpsdirlister.h"
#include "hpsknotdirmodel.h"
#include "hpsdomdirmodel.h"
class HPSDirManager : public QObject
{
    Q_OBJECT
public:
    explicit HPSDirManager(HPSOption &option,QObject *parent = 0);
    ~HPSDirManager();
    bool addDir(const QString  &dir,bool withSub);
    void setModel( QStandardItemModel *model);
    void makeView();
    void removeDir(const QString &dir,bool withSub);
    void saveDirModel();
    void reset();
    const QStringList & dirs();

private:

    void startDirLister(const QString &dir);


    HPSOption &mOption;
    QStandardItemModel *mCurrentModel;
    HPSKnotDirModel mKnotDirModel;
    HPSDomDirModel mDomDirModel;


signals:

public slots:
    void checkWork();
};

#endif // HPSDIRMANAGER_H
