#include "hpsdirmanager.h"
#include <QDebug>

HPSDirManager::HPSDirManager(HPSOption &option, QObject *parent) :
    QObject(parent),mOption(option)
{
    if( mDomDirModel.load( mKnotDirModel ))
        qDebug() << Q_FUNC_INFO << "dirmodele erfolgreich geladen";
    else
        qDebug() << Q_FUNC_INFO << "dirmodele konnte nicht egladen werden";
}
HPSDirManager::~HPSDirManager() {

}


bool HPSDirManager::addDir(const QString  &dir,bool withsub){

    if(withsub)
        startDirLister(dir);
    else
        mKnotDirModel.add(dir);
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



}





void HPSDirManager::startDirLister(const QString &dir)
{
    qDebug() << Q_FUNC_INFO;
    QThread *thread = new QThread();
    HPSDirLister *dirLister = new HPSDirLister( mKnotDirModel,dir);
    dirLister->moveToThread(thread);
    connect(thread,SIGNAL(started()),dirLister,SLOT(startWork()));
    connect(dirLister,SIGNAL(destroyed()),thread,SLOT(quit()));
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    connect(dirLister,SIGNAL(workDone()),this,SLOT(checkWork()));
    thread->start();
}

void HPSDirManager::checkWork()
{
    HPSDirLister *dirLister = (HPSDirLister*) sender();
    qDebug()<<Q_FUNC_INFO << dirLister->dirs().size();
    saveDirModel();

}

void HPSDirManager::saveDirModel()
{
    mDomDirModel.save( mKnotDirModel);
}

void HPSDirManager::reset()
{
    mKnotDirModel.clear();
}




