#ifndef HPSKNOTDIRMODEL_H
#define HPSKNOTDIRMODEL_H

#include <QStandardItem>
#include <QDebug>
class DirKnot{
public:
    DirKnot(): children(QList<DirKnot*>()),path(""),isActive(false),item(NULL){}
    ~DirKnot(){
        QList<DirKnot*> &list = children ;
        const int size = list.size();
        for( int i = 0; i<size;++i){
            delete children.takeFirst();
        }
    }

    QString name;
    QString path;
    QList<DirKnot*> children;
    bool isActive;
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
private:
    void removeDir(DirKnot *parent, QStringList &list,bool withSub);
    void removeAll(DirKnot *parent);
    DirKnot* mRoot;
    QStandardItem *mTmpRoot;



};

#endif // HPSKNOTDIRMODEL_H
