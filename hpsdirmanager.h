#ifndef HPSDIRMANAGER_H
#define HPSDIRMANAGER_H

#include <QObject>
#include <QStandardItemModel>
#include "hpstreecombobox.h"
#include "hpsoption.h"
#include "hpsdirlister.h"
class HPSDirManager : public QObject
{
    Q_OBJECT
public:
    explicit HPSDirManager(HPSOption &option,QObject *parent = 0);
    ~HPSDirManager();
    QStringList addDirsWithSubdirs(const QString &dir);
    bool addDir(const QString  &dir);
    void setModel( QStandardItemModel *model);
    QList<QStandardItem*> makeView();
    //void removeDirs(QStringList dirs);
    void removeDir(const QString &dir,bool withSub);

    const QStringList & dirs();

private:
    void addDirToTree(QList<QStandardItem*> & expandedItems,const QString &dir);
    void addDirToTree(const QString &dir);
    void addDirToList(const QString &dir);
    void removeDirFromList(const QString &dir,bool withSub);
    void subDirsFrom(const QString &dir, QStringList &dirs);

    void removeDirFromTree(const QString &dir,bool withsub);
    void findChildren(const QStandardItem *item, QStringList &list);
    HPSOption &mOption;
    QStandardItemModel *mCurrentModel;
    HPSDirLister *mDirLister;


signals:

public slots:
    void removeDirs(QStringList dirs);
};

#endif // HPSDIRMANAGER_H
