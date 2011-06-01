#include "hpsdirmanager.h"
#include <QDebug>

HPSDirManager::HPSDirManager(HPSOption &option, QObject *parent) :
    QObject(parent),option(option)
{
}
HPSDirManager::~HPSDirManager() {

}


void HPSDirManager::addDir(const QString  &dir){
    if(!option.getOrdner().contains(dir)){
        option.addOrdner(dir);
        option.getComboBoxView()==HPSOption::ListView ? addDirToList(dir):addDirToTree( dir);
        qDebug() << dir << "hinzugefügt";
    }
}

const QStringList & HPSDirManager::dirs()
{
    return option.getOrdner();
}




void HPSDirManager::removeDirs(QStringList dirs)
{
    qDebug() << Q_FUNC_INFO << dirs;
    if ( option.getComboBoxView() == HPSOption::ListView) {
        for (int var = 0; var < dirs.size(); ++var) {
            removeDirFromList(dirs.at(var));
            option.removeOrdner(dirs.at(var));
        }
    } else {
        for (int var = 0; var < dirs.size(); ++var) {
            removeDirFromTree(dirs.at(var));
            option.removeOrdner(dirs.at(var));
        }
    }


}

void HPSDirManager::addDirToTree(const QString &dir){
    qDebug() << __func__ << dir;
    const QStringList folder=dir.split("/",QString::SkipEmptyParts);
    QStandardItem * newItem;
    QStandardItem *parent = currentModel->invisibleRootItem();
    QStandardItem *child;
    QString currentDir;
    bool find = false;
    for (int var = 0; var < folder.size(); ++var) {
        currentDir.append( folder.at(var));
        for (int i = 0; i < parent->rowCount(); ++i) {
            child = parent->child(i);
            if(child->text() == folder.at(var)){
                child->setData(child->data(Qt::UserRole+1).toInt()+1,Qt::UserRole+1);
                if (var == folder.size()-1){
                    child->setEnabled(true);
                    child->setToolTip(QDir::toNativeSeparators(dir));
                }

                find = true;
                parent =child;
                break;
            }
        }
        if (!find) {
            qDebug() << folder.at(var) << "nicht gefunden ";
            newItem = new QStandardItem(folder.at(var));
            newItem->setData(1,Qt::UserRole+1);
            if(var == folder.size()-1){
                newItem->setData(dir,Qt::UserRole);
                newItem->setToolTip(QDir::toNativeSeparators(dir));
            } else {
                newItem->setEnabled(false);
                newItem->setData( currentDir,Qt::UserRole);
            }
            parent->appendRow(newItem);
            parent =newItem;
        } else {
            find = false;
        }
    }
}

void HPSDirManager::addDirToTree(QList<QStandardItem*> & expandedItems,const QString &dir)
{
    qDebug() << __func__ << dir;
    const QStringList folder=dir.split("/",QString::SkipEmptyParts);
    QStandardItem * newItem;
    QStandardItem *parent = currentModel->invisibleRootItem();
    QStandardItem *child;
    QString currentDir;
    bool find = false;
    for (int var = 0; var < folder.size(); ++var) {
        currentDir.append( folder.at(var));
        for (int i = 0; i < parent->rowCount(); ++i) {
            child = parent->child(i);
            if(child->text() == folder.at(var)){
                child->setData(child->data(Qt::UserRole+1).toInt()+1,Qt::UserRole+1);
                if (var == folder.size()-1){
                    child->setEnabled(true);
                    child->setToolTip(QDir::toNativeSeparators(dir));
                }

                find = true;
                parent =child;
                break;
            }
        }
        if (!find) {
            qDebug() << folder.at(var) << "nicht gefunden ";
            newItem = new QStandardItem(folder.at(var));
            newItem->setData(1,Qt::UserRole+1);
            if(var == folder.size()-1){
                newItem->setData(dir,Qt::UserRole);
                newItem->setToolTip(QDir::toNativeSeparators(dir));
                if (option.expandDirs().contains(dir))
                    expandedItems.append(newItem);
            } else {
                newItem->setEnabled(false);
                newItem->setData( currentDir,Qt::UserRole);
                if (option.expandDirs().contains(currentDir))
                    expandedItems.append(newItem);
            }
            parent->appendRow(newItem);
            parent =newItem;
        } else {
            find = false;
        }
    }
}

void HPSDirManager::addDirToList(const QString &dir)
{
    QStandardItem *item = new QStandardItem(QDir::toNativeSeparators(dir));
    item->setData(dir,Qt::UserRole);
    item->setToolTip(QDir::toNativeSeparators(dir));
    currentModel->invisibleRootItem()->appendRow(item);
}

void HPSDirManager::setModel(QStandardItemModel *model)
{
    currentModel = model;
}

QList<QStandardItem*> HPSDirManager::makeView()
{
    const QStringList dirs = option.getOrdner();
    QList<QStandardItem*> expandedItems;
    const int size = dirs.size();

    currentModel->clear();

    if ( option.getComboBoxView() == HPSOption::ListView) {
        qDebug() << "ToList";
        for (int i = 0; i < size; ++i) {
            addDirToList(dirs.at(i));
        }
    } else {
        qDebug() << "toTree";
        for (int i = 0; i < size; ++i) {
            addDirToTree( expandedItems,dirs.at(i));
        }
    }
    return expandedItems;
}

void HPSDirManager::removeDirFromList(const QString &dir)
{

qDebug() << Q_FUNC_INFO << dir;
    const int size = currentModel->rowCount();
    QStandardItem *item;
    for (int i = 0; i < size; ++i) {
        item = currentModel->item(i);
        if(item->text() == QDir::toNativeSeparators(dir)){
            currentModel->removeRow(i);
            break;
        }
    }
}






void HPSDirManager::removeDirFromTree(const QString &dir)
{
    qDebug() << Q_FUNC_INFO << dir;
    const QStringList folder=dir.split("/",QString::SkipEmptyParts);
    QStandardItem *parent = currentModel->invisibleRootItem();
    QStandardItem *child;
    for (int var = 0; var < folder.size(); ++var) {
        for (int i = 0; i < parent->rowCount(); ++i) {
            child = parent->child(i);
            if(child->text() == folder.at(var)){
                child->setData(child->data(Qt::UserRole+1).toInt()-1,Qt::UserRole+1);
                if(child->data(Qt::UserRole+1).toInt() == 0){
                    qDebug() << child->text() << "entfernt";
                    parent->removeRow(i);
                    return;
                }
                parent =child;
                break;
            }
        }
    }
    qDebug() << parent->text() << "disablte";
    parent->setEnabled(false);
    parent->setToolTip("");

}
