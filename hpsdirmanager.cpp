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



void HPSDirManager::addDirToTree(const QString &dir)
{
    qDebug() << __func__ << dir;
    const QStringList folder=dir.split("/",QString::SkipEmptyParts);
    QStandardItem * newItem;
    QStandardItem *parent = currentModel->invisibleRootItem();
    QStandardItem *child;
    bool find = false;
    for (int var = 0; var < folder.size(); ++var) {
        for (int i = 0; i < parent->rowCount(); ++i) {
            child = parent->child(i);
            if(child->text() == folder.at(var)){
                child->setData(child->data(Qt::UserRole+1).toInt()+1,Qt::UserRole+1);
                if (var == folder.size()-1)
                    child->setEnabled(true);
                find = true;
                parent =child;
                break;
            }
        }
        if (!find) {
            qDebug() << folder.at(var) << "nicht egfunden ";
            newItem = new QStandardItem(folder.at(var));
            newItem->setData(1,Qt::UserRole+1);
            if(var == folder.size()-1){
                newItem->setData(dir,Qt::UserRole);
            } else {
                newItem->setEnabled(false);
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
    currentModel->invisibleRootItem()->appendRow(item);
}

void HPSDirManager::setModel(QStandardItemModel *model)
{
    currentModel = model;
}

void HPSDirManager::makeView()
{
    currentModel->clear();
    const QStringList dirs = option.getOrdner();
    const int size = dirs.size();
    if ( option.getComboBoxView() == HPSOption::ListView) {
        qDebug() << "ToList";
        for (int i = 0; i < size; ++i) {
            addDirToList(dirs.at(i));
        }
    } else {
        qDebug() << "toTree";
        for (int i = 0; i < size; ++i) {
            addDirToTree( dirs.at(i));
        }
    }
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

}
