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
    bool removeDir(const QString &dir);

    const QStringList & dirs();
    QStandardItem * getCurrentItem();

private:
    void showSubTree(QString str,HPSDirKnoten*knote);
    void deleteKnoten(HPSDirKnoten *knoten);
    void remove( QStringList & dirs,HPSDirKnoten *knoten);
    void makeTree(const QString &currentDir,QStandardItem *item,HPSDirKnoten *knoten,QStandardItemModel*tree);
    void makeList(const QString &currentDir,HPSDirKnoten *knoten,QStandardItemModel *model);
    void createTree(const QString &dir);
    HPSDirKnoten *root;
    HPSOption &option;
    QStandardItem *currentItem;
    QStandardItemModel *currentModel;


signals:

public slots:

};

#endif // HPSDIRMANAGER_H
