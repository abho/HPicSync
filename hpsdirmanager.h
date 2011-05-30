#ifndef HPSDIRMANAGER_H
#define HPSDIRMANAGER_H

#include <QObject>
#include <QStandardItemModel>
#include "hpsdirknoten.h"
#include "hpstreecombobox.h"
#include "hpsoption.h"
class HPSDirManager : public QObject
{
    Q_OBJECT
public:
    explicit HPSDirManager(HPSOption &option,QObject *parent = 0);
    ~HPSDirManager();
    void addDir(const QString  &dir);
    void showTree();
    void makeTreeView(const QString &currentDir,QStandardItemModel *model);
    void makeListView(const QString &currentDir,QStandardItemModel *model);
    void setModel( QStandardItemModel *model);
    void makeView();
    bool removeDir(const QString &dir);

    const QStringList & dirs();
    QStandardItem * getCurrentItem();

private:
    void showSubTree(QString str,HPSDirKnoten*knote);
    void deleteKnoten(HPSDirKnoten *knoten);
    void remove( QStringList & dirs,HPSDirKnoten *knoten);
    void makeTree(const QString &currentDir,QStandardItem *item,HPSDirKnoten *knoten);
    void makeList(const QString &currentDir,HPSDirKnoten *knoten);
    void add(const QString& dir);
    void addDirToTree(const QString &dir);
    void addDirToList(const QString &dir);
    void createTree(const QString &dir);
    void creatItemLists(const int);
    void insertItemListIntoView(const int);
    HPSDirKnoten *root;
    HPSOption &option;
    QStandardItem *currentItem;
    QStandardItemModel *currentModel;
    QList<QStandardItem*> treeItems;
    QList< QStandardItem * > listItems;


signals:

public slots:
    void removeDirs(QStringList dirs);
};

#endif // HPSDIRMANAGER_H
