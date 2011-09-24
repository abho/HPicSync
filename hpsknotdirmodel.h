#ifndef HPSKNOTDIRMODEL_H
#define HPSKNOTDIRMODEL_H

#include <QStandardItem>
#include <QDir>
#include <QDebug>
class DirKnot{
public:
    DirKnot(): children(QList<DirKnot*>()),item(NULL),hasFiles(false){}
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
    bool hasFiles;

    static void activateKnot(DirKnot *knot,const QString &dir,bool hasFiles){

    }
};
class HPSKnotDirModel
{
public:
    HPSKnotDirModel();
    ~HPSKnotDirModel();
    void add(const QString &str,bool hasFiles);
    bool remove(const QString &str);
    bool contains(const QString &str);
    void removeWithSubs(const QString &str);
    void setTreeView(bool isTreeView,QStandardItem *item);
    void clear();
    QStringList getWholeDirList();
    DirKnot * root();
    void setDirHasFiles(const QString &dir,bool hasFiles);
    void activateKnot(DirKnot *knot,const QString &dir,bool hasFiles);
    void deactiveKnot(DirKnot *knot);
    static DirKnot * creatNewActiveKnot(const QString &name, bool hasFiles,const QString &path,const bool isExpanded);
    static DirKnot * creatNewDeactiveKnot(const QString &name,const bool isExpanded);
private:
    void removeDir(DirKnot *parent, QStringList &list,bool withSub);
    void removeAll(DirKnot *parent);
    void makeListView(DirKnot *parent,QStandardItem *root);
    void makeDirList(DirKnot *parent,QStringList & dirList);
    DirKnot* findKnot(const QString &dir);
    DirKnot* mRoot;
    QStandardItem *mTmpRoot;



};

#endif // HPSKNOTDIRMODEL_H
