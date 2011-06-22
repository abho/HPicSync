#include "hpsknotdirmodel.h"


HPSKnotDirModel::HPSKnotDirModel()
{
    mRoot = new DirKnot();
    mRoot->name ="";
    mTmpRoot = new QStandardItem("");
    mRoot->item = mTmpRoot;

}

HPSKnotDirModel::~HPSKnotDirModel(){
    delete mTmpRoot;
    delete mRoot;
}

void HPSKnotDirModel::add(const QString &str)
{

    DirKnot *parent = mRoot;
    QList<DirKnot*> children;
    QStringList list= str.split('/',QString::SkipEmptyParts);
    DirKnot *child;
    int i;
    bool found = true;
    const int size = list.size();
    for (int var = 0; var < size; ++var) {
        if(found){
            children = parent->children;
            const int length = children.size();
            for ( i = 0; i < length; ++i) {
                child = children.at(i);
                if(child->name == list.at(var)) {
                    if(var == size-1){
                        child->isActive=true;
                        child->path = str;
                        child->item->setEnabled(true);
                        child->item->setData(str,Qt::UserRole);                        
                        child->item->setToolTip(str);
                    }
                    break;
                }
            }
            if(i == length){
                found = false;
                child = new DirKnot;
                child->name = list.at(var);
                child->item = new QStandardItem(child->name);
                child->item->setData(false,Qt::UserRole+1);

                parent->item->appendRow(child->item);
                if(var == size-1){
                    child->isActive = true;
                    child->item->setEnabled(true);
                    child->item->setData(str,Qt::UserRole);
                    child->path = str;
                    child->item->setToolTip(str);
                } else {
                    child->item->setEnabled(false);
                }
                parent->children.append(child);
            }

        } else {
            child = new DirKnot;
            child->name = list.at(var);
            child->item = new QStandardItem(child->name);
            child->item->setData(false,Qt::UserRole+1);
            parent->item->appendRow(child->item);
            if(var == size-1){
                child->isActive = true;
                child->path = str;
                child->item->setEnabled(true);
                child->item->setData(str,Qt::UserRole);
                child->item->setToolTip(str);
            } else {
                child->item->setEnabled(false);
            }
            parent->children.append(child);
        }
        parent = child;
    }
}

bool HPSKnotDirModel::contains(const QString &str)
{
    DirKnot *parent = mRoot;
    QList<DirKnot*> children;
    QStringList list= str.split('/',QString::SkipEmptyParts);
    DirKnot *child;
    int i;
    const int size = list.size();
    for (int var = 0; var < size; ++var) {
        children = parent->children;
        const int length = children.size();
        for ( i = 0; i < length; ++i) {
            child = children.at(i);
            if(child->name == list.at(var)) {
                break;
            }
        }
        if(i == length){
            return false;
        }
        parent = child;
    }
    if(parent->isActive)
        return true;
    else
        return false;
}

bool HPSKnotDirModel::remove(const QString &str)
{
    QStringList list = str.split('/',QString::SkipEmptyParts);
    if(!list.isEmpty()){
        removeDir(mRoot,list,false);
    }
    return true;
}
void HPSKnotDirModel::removeWithSubs(const QString &str)
{
    QStringList list = str.split('/',QString::SkipEmptyParts);
    if(!list.isEmpty()){
        removeDir(mRoot,list,true);
    }
}

void HPSKnotDirModel::removeDir(DirKnot *parent, QStringList &list,bool withSub)
{
    const QString name = list.takeFirst();
    QList<DirKnot*> children =parent->children;
    DirKnot* child;
    bool last = false;
    int var;
    const int size = children.size();
    for (var = 0; var < size; ++var) {
        child = children.at(var);
        if(child->name == name){
            if(!list.isEmpty()){
                removeDir(child,list,withSub);
            }else {
                child->isActive = false;
                child->item->setEnabled(false);
                last=true;
            }
            break;
        }
    }
    if(child->children.size()==0 || (last&&withSub)){
        parent->item->takeRow(var);
        delete parent->children.takeAt(var);
    }
}




void HPSKnotDirModel::clear()
{
    if(mRoot->item->hasChildren()){
        const int size = mRoot->item->rowCount();
        for (int var = 0; var < size; ++var) {
            delete mRoot->item->takeChild(0,0);
        }
    }
}

DirKnot * HPSKnotDirModel::root()
{
    return mRoot;
}

void HPSKnotDirModel::setTreeView(bool isTreeView,QStandardItem *item)
{
    qDebug() << Q_FUNC_INFO << isTreeView <<item->text();
    QStandardItem *tmpItem;
    if( isTreeView){
        const int size = mTmpRoot->rowCount();
        for(  int i = 0 ;  i < size ; ++i ) {
            tmpItem = mTmpRoot->takeRow(0).first();
            qDebug() << tmpItem->text();
            item->appendRow(tmpItem);
        }
        mRoot->item = item;
    }else{
        const int size = item->rowCount();
        for(  int i = 0 ;  i < size ; ++i ) {
            tmpItem = item->takeRow(0).first();
            qDebug() << tmpItem->text();
            mTmpRoot->appendRow(tmpItem);
        }
        mRoot->item = mTmpRoot;
    }
}


