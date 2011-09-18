#include "hpsdirmanager.h"
#include <QDebug>


/*!

  */
HPSDirManager::HPSDirManager(HPSThreadManager &threadManager,HPSDBHandler &dbHandler,HPSOption &option, QObject *parent) :
    QObject(parent),mOption(option),mThreadManager(threadManager)
{
    if( mDomDirModel.load( mKnotDirModel ))
        qDebug() << Q_FUNC_INFO << "dirmodele erfolgreich geladen";
    else
        qDebug() << Q_FUNC_INFO << "dirmodele konnte nicht egladen werden";

    mThreadManager.initDirLister(mKnotDirModel);
    mThreadManager.initDirChecker(mOption,dbHandler);
    HPSDirChecker * dirChecker = mThreadManager.dirChecker();
    HPSDirLister * dirLister = mThreadManager.dirLister();

    connect(dirChecker,SIGNAL(dirChecked(QString)),this,SLOT(finishAddDir(QString)));
    connect(dirLister,SIGNAL(dirDone(QString)),dirChecker,SLOT(checkDir(QString)));
    connect(this,SIGNAL(checkDir(QString)),dirChecker,SLOT(checkDir(QString)));
    connect(this,SIGNAL(startDirLister(QString)),dirLister,SLOT(startWork(QString)));
    connect(dirLister,SIGNAL(workDone()),this,SLOT(checkWork()));
}
HPSDirManager::~HPSDirManager() {

}
bool HPSDirManager::startAddDir(const QString  &dir,bool withsub){
qDebug() << Q_FUNC_INFO << dir <<withsub;
    if(withsub)
        emit startDirLister(dir);
    else{
        if(!mKnotDirModel.contains(dir))
            emit checkDir(dir);
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

void HPSDirManager::checkWork()
{
    // HPSDirLister *dirLister = (HPSDirLister*) sender();
    qDebug()<<Q_FUNC_INFO ;
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
void HPSDirManager::finishAddDir( QString str)
{
    mKnotDirModel.add(str);
}
HPSKnotDirModel & HPSDirManager::knotDirModel()
{
    return mKnotDirModel;
}







