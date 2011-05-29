#include "hpsdirmanager.h"
#include <QDebug>

HPSDirManager::HPSDirManager(HPSOption &option, QObject *parent) :
    QObject(parent),option(option),currentItem(NULL)
{
    this->root = new HPSDirKnoten();
    this->root->setName("_root");
    qDebug() <<"get ordner size"<< option.getOrdner().size();
    for (int var = 0; var < option.getOrdner().size(); ++var) {
        createTree(option.getOrdner().at(var));
    }
}
HPSDirManager::~HPSDirManager() {
    this->deleteKnoten(this->root);
}

void HPSDirManager::deleteKnoten(HPSDirKnoten *knoten){
    const QList<HPSDirKnoten*> &list =knoten->getChildren();
    for(int i = 0;i < list.size();i++){
        this->deleteKnoten(list.at(i));
    }
    delete knoten;
}

void HPSDirManager::showTree(){
    this->showSubTree("",this->root);
}
void HPSDirManager::makeListView(const QString &currentDir,QStandardItemModel *model){
    currentItem = NULL;
    qDebug() << "makeListView";
    model->clear();
    makeList(currentDir,root,model);
}
void HPSDirManager::makeList(const QString &currentDir,HPSDirKnoten *knoten,QStandardItemModel *model){
    const QList<HPSDirKnoten *> &list = knoten->getChildren();
    if(knoten->getActive() && knoten->getName()!="_root"){
        qDebug() << "item erstell:" << knoten->getDirname();
        QStandardItem *newItem = new QStandardItem(knoten->getDirname());
        newItem->setData(knoten->getDirname(),Qt::UserRole);
        if( knoten->getDirname() == currentDir)
            currentItem = newItem;
        model->appendRow(newItem);
    }
    for( int i = 0; i < list.size() ; i++ ){
        makeList(currentDir,list.at(i),model);
    }

}

void HPSDirManager::makeTreeView(const QString &currentDir,QStandardItemModel *model){
    qDebug() << "maketreeView";
    currentItem = NULL;
    model->clear();
    this->makeTree(currentDir,NULL,this->root,model);

}

void HPSDirManager::makeTree(const QString &currentDir,QStandardItem *item,HPSDirKnoten *knoten, QStandardItemModel *tree){
    qDebug()<< "makeTree"  <<  item<<knoten->getName();
    const QList<HPSDirKnoten *> &list = knoten->getChildren();
    HPSDirKnoten *knot;
    QStandardItem *newItem;
    if(list.size() != 0){
        for (int i = 0 ; i< list.size() ; i++){
            knot = list.at(i);
            newItem = new QStandardItem(knot->getName());
            if(!knot->getActive()){
                newItem->setEnabled(false);
            }else {
                newItem->setData(knot->getDirname(),Qt::UserRole);
                if(knot->getDirname() == currentDir)
                    currentItem = newItem;
            }
            if(item == NULL){
                tree->appendRow(newItem);
                qDebug() << "item erstelt mit:" << knot->getName() << "Null";
            }else {
                item->appendRow(newItem);
                qDebug() << "item erstelt mit:" << knot->getName() << item->text();            }
            makeTree(currentDir,newItem,knot,tree);
        }
    }
}

void HPSDirManager::showSubTree(QString str,HPSDirKnoten*knote){
    const QList<HPSDirKnoten*> &list =knote->getChildren();
    for(int i = 0;i < list.size();i++){
    }
    for(int i = 0;i < list.size();i++){
        this->showSubTree(str+"/"+knote->getName(),list.at(i));
    }
}
void HPSDirManager::addDir(const QString  &dir){

    if(!option.getOrdner().contains(dir)){
        option.addOrdner(dir);
        createTree(dir);
    }
}

const QStringList & HPSDirManager::dirs()
{
    return option.getOrdner();
}

void HPSDirManager::createTree(const QString &dir)
{
    qDebug() << dir;
    QStringList listTeile = dir.split("/",QString::SkipEmptyParts);
    HPSDirKnoten *parent,*child;
    parent =this->root;
    bool end = false;
    int i;
    for(i = 0; i< listTeile.size();i++){
        if(!end){
            const QList<HPSDirKnoten*> list = parent->getChildren();
            qDebug() << list.size();
            int j;
            for( j= 0;j < list.size();j++){
                child = list.at(j);
                if(child->getName() == listTeile.at(i)){
                    child->inkrementCount();
                    if(i == listTeile.size()-1 &&child->getActive() == false ){
                        child->setActive(true);
                        child->setDirname(dir);
                    }

                    qDebug() << listTeile.at(i) << "existiert schon" << ":" << child->getActive();
                    parent =child;
                    break;
                } qDebug() << list.at(j)->getName();
            }
            if(j==list.size()){
                end=true;
                HPSDirKnoten *knoten = new HPSDirKnoten();
                knoten->setName(listTeile.at(i));
                if( i == listTeile.size()-1){
                    knoten->setActive(true);
                    knoten->setDirname(dir);
                } else {
                    knoten->setActive(false);
                }
                parent->addChild(knoten);
                parent=knoten;
                qDebug() << knoten->getName() << "hinzugefuegt" << ":" << knoten->getActive();
            }
        }else {
            HPSDirKnoten *knoten = new HPSDirKnoten();
            knoten->setName(listTeile.at(i));
            if( i == listTeile.size()-1){
                knoten->setActive(true);
                knoten->setDirname(dir);
            } else {
                knoten->setActive(false);
            }
            parent->addChild(knoten);
            parent=knoten;
            qDebug() << knoten->getName() << "hinzugefuegt" << ":" << knoten->getActive();
        }
    }
}

QStandardItem * HPSDirManager::getCurrentItem()
{
    return currentItem;
}

bool HPSDirManager::removeDir(const QString &dir)
{
    if(option.getOrdner().contains(dir)){
        QStringList ordner = dir.split('/',QString::SkipEmptyParts);
        remove(ordner,root);
        option.removeOrdner(dir);
        return true;
    } else {
        return false;
    }
}

void HPSDirManager::remove( QStringList &dirs,HPSDirKnoten *knoten)
{    if(!dirs.isEmpty()){
        QString ordner = dirs.takeFirst();
       const QList<HPSDirKnoten*> &list = knoten->getChildren();
        HPSDirKnoten *knot;
        for (int var = 0; var < list.size(); ++var) {
            knot = list.at(var);
            if (knot->getName() == ordner){
                remove(dirs,knot);
                knot->dekrementCount();
                if(knot->count() == 0){
                    qDebug() << knot->getName() << "entfernt";
                    knoten->removeChild (knot);
                    break;
                }
            }
        }
    }
}

void HPSDirManager::removeDirs(QStringList dirs)
{
    for (int var = 0; var < dirs.size(); ++var) {
        removeDir(dirs.at(var));
    }
}
