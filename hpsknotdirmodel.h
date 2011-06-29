#ifndef HPSKNOTDIRMODEL_H
#define HPSKNOTDIRMODEL_H

#include <QStandardItem>
#include <QDir>
#include <QDebug>
class DirKnot{
public:
    DirKnot(): children(QList<DirKnot*>()),item(NULL){}
    ~DirKnot(){
        QList<DirKnot*> &list = children ;
        const int size = list.size();
        for( int i = 0; i<size;++i){
            delete children.takeFirst();
        }
    }

    QString name;
    QList<DirKnot*> children;
    QStandardItem* item;
};
class HPSKnotDirModel
{
public:
    HPSKnotDirModel();
    ~HPSKnotDirModel();
    void add(const QString &str);
    bool remove(const QString &str);
    bool contains(const QString &str);
    void removeWithSubs(const QString &str);
    void setTreeView(bool isTreeView,QStandardItem *item);
    void clear();
    DirKnot * root();

    static DirKnot * creatNewActiveKnot(const QString &name,const QString &path,const bool isExpanded);
    static DirKnot * creatNewDeactiveKnot(const QString &name,const bool isExpanded);
private:
    void removeDir(DirKnot *parent, QStringList &list,bool withSub);
    void removeAll(DirKnot *parent);
    void makeListView(DirKnot *parent,QStandardItem *root);
    DirKnot* mRoot;
    QStandardItem *mTmpRoot;



};

#endif // HPSKNOTDIRMODEL_H
