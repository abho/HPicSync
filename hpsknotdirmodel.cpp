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

void HPSKnotDirModel::add(const QString &str,bool hasFiles)
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
                        activateKnot(child,str,hasFiles);
                    }
                    break;
                }
            }
            if(i == length){
                found = false;
                if(var == size-1)
                    child = creatNewActiveKnot(list.at(var),hasFiles,str,false);
                else
                    child = creatNewDeactiveKnot(list.at(var),false);
                parent->item->appendRow(child->item);
                parent->children.append(child);
            }
        } else {
            if(var == size-1)
                child = creatNewActiveKnot(list.at(var),hasFiles,str,false);
            else
                child = creatNewDeactiveKnot(list.at(var),false);
            parent->item->appendRow(child->item);
            parent->children.append(child);
        }
        parent = child;
    }
}

bool HPSKnotDirModel::contains(const QString &str)
{
    DirKnot *knot = findKnot(str);
    if(knot!=NULL){
        if (knot->item->isEnabled())
            return true;
    }
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
    QList<DirKnot*> children = parent->children;
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
                qDebug() << Q_FUNC_INFO << child->item->data(Qt::UserRole).toString();
                deactiveKnot(child);
                last=true;
            }
            break;
        }
    }
    if(child->children.size()==0 || (last&&withSub)){
        delete parent->item->takeRow(var).first();
        delete parent->children.takeAt(var);
    }
}

void HPSKnotDirModel::deactiveKnot(DirKnot *knot)
{
    knot->item->setEnabled(false);
    knot->item->setBackground(QBrush(Qt::white));
    knot->hasFiles =false;
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
        const int count = item->rowCount();
        for ( int i = 0 ; i < count ; ++i){
            delete item->takeRow(0).first();
        }
        qDebug() << item->rowCount();
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
        makeListView(mRoot,item);
    }
}

void HPSKnotDirModel::activateKnot(DirKnot *knot, const QString &dir, bool hasFiles)
{
    knot->item->setEnabled(true);
    knot->item->setData(QDir::fromNativeSeparators(dir),Qt::UserRole);
    knot->item->setToolTip(QDir::toNativeSeparators(dir));
    if(hasFiles){
        knot->hasFiles = true;
        knot->item->setBackground(QBrush(QColor(0,255,0,40)));
    }
}
DirKnot * HPSKnotDirModel::creatNewActiveKnot(const QString &name,bool hasFiles, const QString &path, const bool isExpanded)
{
    DirKnot *newKnot = new DirKnot();
    newKnot->name = name;
qDebug() << Q_FUNC_INFO << hasFiles;
    QStandardItem *newItem = new QStandardItem(name);
    newItem->setEnabled(true);
    newItem->setToolTip(QDir::toNativeSeparators(path));
    newItem->setData(isExpanded,Qt::UserRole+1);
    newItem->setData(QDir::fromNativeSeparators(path),Qt::UserRole);
    if(hasFiles){
        newKnot->hasFiles = true;
        newItem->setBackground(QBrush(QColor(0,255,0,40)));
    } else {
        newKnot->hasFiles = false;
        newItem->setBackground(QBrush(QColor(0,0,0,0)));
    }
    newKnot->item = newItem;

    return newKnot;
}

DirKnot * HPSKnotDirModel::creatNewDeactiveKnot(const QString &name, const bool isExpanded)
{
    DirKnot *newKnot = new DirKnot();
    newKnot->name = name;

    QStandardItem *newItem = new QStandardItem(name);
    newItem->setEnabled(false);
    newItem->setData(isExpanded,Qt::UserRole+1);
    newKnot->item = newItem;

    return newKnot;
}

void HPSKnotDirModel::makeListView(DirKnot *parent,QStandardItem *root)
{
    const QList<DirKnot*> &list = parent->children;
    const int size = list.size();
    DirKnot *child;
    QStandardItem *newItem,*oldItem;
    QString path;
    for ( int i = 0 ; i < size ; ++i){
        child = list.at(i);
        oldItem= child->item;
        if( oldItem->isEnabled()){
            path = oldItem->data(Qt::UserRole).toString();
            newItem = new QStandardItem(QDir::toNativeSeparators(path));
            newItem->setData( path,Qt::UserRole);
            newItem->setToolTip(path);
            if(child->hasFiles)
                newItem->setBackground(QBrush(QColor(0,255,0,40)));
            root->appendRow(newItem);
        }
        makeListView(child,root);
    }
}

QStringList HPSKnotDirModel::getWholeDirList()
{
    qDebug() << Q_FUNC_INFO;
    QStringList dirList;
    QList<DirKnot*> &children = mRoot->children;
    const int size = children.size();
    for(  int i = 0 ;  i < size ; ++i ) {
        makeDirList(children.at(i),dirList);
    }
    return dirList;

}

void HPSKnotDirModel::makeDirList(DirKnot *parent, QStringList &dirList)
{    
    if(parent->item->isEnabled())
        dirList << parent->item->data(Qt::UserRole).toString();
    QList<DirKnot*> &children = parent->children;
    const int size = children.size();
    for(  int i = 0 ;  i < size ; ++i ) {
        makeDirList(children.at(i),dirList);
    }

}


DirKnot * HPSKnotDirModel::findKnot(const QString &dir)
{

    DirKnot *parent = mRoot;
    QList<DirKnot*> children;
    QStringList list= dir.split('/',QString::SkipEmptyParts);
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
            return NULL;
        }
        parent = child;
    }
    return parent;
}

void HPSKnotDirModel::setDirHasFiles(const QString &dir, bool hasFiles)
{
    DirKnot *knot =findKnot(dir);
    if(knot != NULL){
        knot->hasFiles = hasFiles;
        if( hasFiles)
            knot->item->setBackground(QBrush(QColor(0,255,0,40)));
        else
            knot->item->setBackground(QBrush(Qt::white));
    }
}






