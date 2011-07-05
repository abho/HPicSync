#include "hpsdirmanager.h"
#include <QDebug>

HPSDirManager::HPSDirManager(HPSThumbManager &thumbManager,HPSOption &option, QObject *parent) :
    QObject(parent),mOption(option),mThumbManager(thumbManager)
{
    if( mDomDirModel.load( mKnotDirModel ))
        qDebug() << Q_FUNC_INFO << "dirmodele erfolgreich geladen";
    else
        qDebug() << Q_FUNC_INFO << "dirmodele konnte nicht egladen werden";
}
HPSDirManager::~HPSDirManager() {

}


bool HPSDirManager::startAddDir(const QString  &dir,bool withsub){

    if(withsub)
        startDirLister(dir);
    else{
        if(!mKnotDirModel.contains(dir))
            checkDir(dir);
    }
    return true;
}

const QStringList & HPSDirManager::dirs()
{
    return mOption.dirs();
}

void HPSDirManager::setModel(QStandardItemModel *model)
{
    mCurrentModel = model;
}

void HPSDirManager::makeView()
{


    //mCurrentModel->clear();

    if ( mOption.getComboBoxView() == HPSOption::ListView) {
        // qDebug() << "ToList";
        mKnotDirModel.setTreeView(false,mCurrentModel->invisibleRootItem());
    } else {
        //qDebug() << "toTree";
        mKnotDirModel.setTreeView(true,mCurrentModel->invisibleRootItem());
    }

}





void HPSDirManager::removeDir(const QString &dir, bool withSub)
{

    qDebug() <<  Q_FUNC_INFO;
    if(withSub)
        mKnotDirModel.removeWithSubs(dir);
    else
        mKnotDirModel.remove(dir);

    if ( mOption.getComboBoxView() == HPSOption::ListView) {
        QList<QStandardItem*> list;
        QStandardItem *item;
        if(withSub)
            list = mCurrentModel->findItems(QDir::toNativeSeparators(dir),Qt::MatchStartsWith);
        else
            list = mCurrentModel->findItems(QDir::toNativeSeparators(dir));

        const int size = list.size();
        for(  int i = 0 ;  i < size ; ++i ) {
            qDebug()<< Q_FUNC_INFO << list.at(i)->data(Qt::UserRole).toString();
            mCurrentModel->removeRow(list.at(i)->row());

        }
    }
}





void HPSDirManager::startDirLister(const QString &dir)
{
    qDebug() << Q_FUNC_INFO;
    mOption.setDirFromDirlister(dir);
    QThread *thread = new QThread();
    HPSDirLister *dirLister = new HPSDirLister( mKnotDirModel,dir);
    dirLister->moveToThread(thread);
    connect(thread,SIGNAL(started()),dirLister,SLOT(startWork()));
    connect(dirLister,SIGNAL(destroyed()),thread,SLOT(quit()));
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    connect(dirLister,SIGNAL(workDone()),this,SLOT(checkWork()));
    connect(dirLister,SIGNAL(dirDone(QString)),this,SLOT(checkDir(QString)));
    thread->start();
}

void HPSDirManager::checkWork()
{
    // HPSDirLister *dirLister = (HPSDirLister*) sender();
    qDebug()<<Q_FUNC_INFO ;
    //saveDirModel();
    mOption.setDirFromDirlister("");

}

void HPSDirManager::saveDirModel()
{
    mDomDirModel.save( mKnotDirModel);
}

void HPSDirManager::reset()
{
    mKnotDirModel.clear();
}

void HPSDirManager::checkDir(QString str)
{
    mThumbManager.creatThumbs(str,false);
}

void HPSDirManager::finishAddDir( QString str)
{
    mKnotDirModel.add(str);
}





