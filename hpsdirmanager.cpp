#include "hpsdirmanager.h"
#include <QDebug>

HPSDirManager::HPSDirManager(HPSOption &option, QObject *parent) :
    QObject(parent),mOption(option),mDirLister(NULL)
{
}
HPSDirManager::~HPSDirManager() {

}


bool HPSDirManager::addDir(const QString  &dir){

    if(!mOption.dirs().contains(dir)){
        mOption.addDir(dir);
        mOption.getComboBoxView()==HPSOption::ListView ? addDirToList(dir):addDirToTree( dir);
        // qDebug() << dir << "hinzugefügt";
        return true;
    } else
        return false;
}

const QStringList & HPSDirManager::dirs()
{
    return mOption.dirs();
}




void HPSDirManager::removeDirs(QStringList dirs)
{
    /*//qDebug() << Q_FUNC_INFO << dirs;
    if ( mOption.getComboBoxView() == HPSOption::ListView) {
        for (int var = 0; var < dirs.size(); ++var) {
            removeDirFromList(dirs.at(var));
            mOption.removeDir(dirs.at(var));
        }
    } else {
        for (int var = 0; var < dirs.size(); ++var) {
            removeDirFromTree(dirs.at(var));
            mOption.removeDir(dirs.at(var));
        }
    }
*/

}

void HPSDirManager::addDirToTree(const QString &dir){
    //qDebug() << __func__ << dir;
    const QStringList folder=dir.split("/",QString::SkipEmptyParts);
    QStandardItem * newItem;
    QStandardItem *parent = mCurrentModel->invisibleRootItem();
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
                    child->setData(dir,Qt::UserRole);
                    child->setToolTip(QDir::toNativeSeparators(dir));
                }

                find = true;
                parent =child;
                break;
            }
        }
        if (!find) {
            //qDebug() << folder.at(var) << "nicht gefunden ";
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
    //qDebug() << __func__ << dir;
    const QStringList folder=dir.split("/",QString::SkipEmptyParts);
    QStandardItem * newItem;
    QStandardItem *parent = mCurrentModel->invisibleRootItem();
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
                    child->setData(dir,Qt::UserRole);
                    child->setToolTip(QDir::toNativeSeparators(dir));
                }

                find = true;
                parent =child;
                break;
            }
        }
        if (!find) {
            //qDebug() << folder.at(var) << "nicht gefunden ";
            newItem = new QStandardItem(folder.at(var));
            newItem->setData(1,Qt::UserRole+1);
            if(var == folder.size()-1){
                newItem->setData(dir,Qt::UserRole);
                newItem->setToolTip(QDir::toNativeSeparators(dir));
                if (mOption.expandDirs().contains(dir))
                    expandedItems.append(newItem);
            } else {
                newItem->setEnabled(false);
                newItem->setData( currentDir,Qt::UserRole);
                if (mOption.expandDirs().contains(currentDir))
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
    mCurrentModel->invisibleRootItem()->appendRow(item);
}

void HPSDirManager::setModel(QStandardItemModel *model)
{
    mCurrentModel = model;
}

QList<QStandardItem*> HPSDirManager::makeView()
{
    const QStringList dirs = mOption.dirs();
    QList<QStandardItem*> expandedItems;
    const int size = dirs.size();

    mCurrentModel->clear();

    if ( mOption.getComboBoxView() == HPSOption::ListView) {
        // qDebug() << "ToList";
        for (int i = 0; i < size; ++i) {
            addDirToList(dirs.at(i));
        }
    } else {
        //qDebug() << "toTree";
        for (int i = 0; i < size; ++i) {
            addDirToTree( expandedItems,dirs.at(i));
        }
    }
    return expandedItems;
}

void HPSDirManager::removeDirFromList(const QString &dir,bool withSub)
{
    const int size = mCurrentModel->rowCount();
    //qDebug() << Q_FUNC_INFO << dir;
    QStandardItem *item;
    if(!withSub){
        for (int i = 0; i < size; ++i) {
            item = mCurrentModel->item(i);
            if(item->text() == QDir::toNativeSeparators(dir)){
                mCurrentModel->removeRow(i);
                mOption.removeDir(dir);
                qDebug() << dir << "entfernt";
                break;
            }
        }
    } else {
        bool found;
        QStringList list = dir.split("/");
        const int length = list.size();
        QStringList tmpList;
        for (int i = 0; i < size; ++i) {
            tmpList =QDir::fromNativeSeparators(mCurrentModel->item(i)->text()).split("/");
            if(tmpList.size()>=length){
                found =true;
                for (int var = 0; var < length; ++var) {
                    if(tmpList.at(var) != list.at(var)){
                        found=false;
                        break;
                    }
                }
                if(found){
                    mCurrentModel->removeRow(i);
                    mOption.removeDir(QDir::fromNativeSeparators(mCurrentModel->item(i)->text()));
                    qDebug() << QDir::fromNativeSeparators(mCurrentModel->item(i)->text())<< "entfernt";
                }
            }
        }
    }

}






void HPSDirManager::removeDirFromTree(const QString &dir,bool withSub)
{
    //qDebug() << Q_FUNC_INFO << dir;
    const QStringList folder=dir.split("/",QString::SkipEmptyParts);
    QStandardItem *parent = mCurrentModel->invisibleRootItem();
    QStandardItem *child;
    bool found = true;
    for (int var = 0; var < folder.size(); ++var) {
        found = false;
        for (int i = 0; i < parent->rowCount(); ++i) {
            child = parent->child(i);
            if(child->text() == folder.at(var)){
                found = true;
                child->setData(child->data(Qt::UserRole+1).toInt()-1,Qt::UserRole+1);
                if(child->data(Qt::UserRole+1).toInt() == 0){
                    qDebug() <<child->data(Qt::UserRole).toString()<< child->text() << "entfernt";
                    parent->removeRow(i);
                    mOption.removeDir(dir);
                    return;
                }
                parent =child;
                break;
            }
        }
    }
    if(found){
        if(withSub){
          QStringList list;
          findChildren(parent,list);
          parent->parent()->removeRow(parent->row());
          qDebug() << list << "entfernt";
          mOption.removeDirs(list);
        }else {
           qDebug() << parent->text() << "disablte";
            parent->setEnabled(false);
            parent->setToolTip("");
            mOption.removeDir(dir);
        }

    }

}

QStringList HPSDirManager::addDirsWithSubdirs(const QString &dir)
{
    QStringList cDirs;
    if (!dirs().contains(dir))
        cDirs.append(dir);
    subDirsFrom(dir,cDirs);
    mOption.appendDirs(cDirs);
    const int size = cDirs.size();
    if(mOption.getComboBoxView()==HPSOption::ListView){
        for (int i = 0; i < size; ++i) {
            addDirToList(cDirs.at(i));
        }
    } else {
        for (int i = 0; i < size; ++i) {
            addDirToTree( cDirs.at(i));
        }
    }

    return cDirs;
}
void HPSDirManager::subDirsFrom(const QString &dir, QStringList &cDirs)
{
    QDir cDir(dir);
    QStringList list = cDir.entryList(QStringList()<< "*",QDir::Dirs|QDir::NoSymLinks|QDir::NoDotAndDotDot);
    const int size = list.size();
    for (int var = 0; var < size; ++var) {
        subDirsFrom(QString(dir+"/"+list.at(var)),cDirs);
        if(!dirs().contains(dir+"/"+list.at(var)))
            cDirs.append(dir+"/"+list.at(var));
    }

}

void HPSDirManager::removeDir(const QString &dir, bool withSub)
{

        if(mOption.getComboBoxView() == HPSOption::ListView)
            removeDirFromList(dir,withSub);
        else
            removeDirFromTree(dir, withSub);


}

void HPSDirManager::findChildren(const QStandardItem *item, QStringList &list)
{
    const int size = item->rowCount();
    for (int var = 0; var < size; ++var) {
        findChildren(item->child(var),list);
    }
    list.append(item->data(Qt::UserRole).toString());
}
