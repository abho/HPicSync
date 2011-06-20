#include "hpshashsaver.h"

HPSHashSaver::HPSHashSaver(HPSDBHandler *dbHandler,const QVector<HPSThumb> &imageVector,const QString &folder,QObject *parent ):
    HPSWorkerClass(parent),mImageVec(imageVector),mFolder(folder),mDatabaseHandler( dbHandler)
{
}

HPSHashSaver::~HPSHashSaver(){
    qDebug() << Q_FUNC_INFO << "tot";
}

void HPSHashSaver::start()
{
    QElapsedTimer timer;
    timer.start();

    if( mDatabaseHandler != NULL){
        mDatabaseHandler->startTransaction();
        const int size = mImageVec.size();

        for (int var = 0; var < size; ++var) {
            if(mEx){
                   break;
            }
            const HPSThumb &thumb = mImageVec.at(var);
                if(!thumb.error) {
                    //qDebug() << QApplication::applicationDirPath()+"/.thumbs/"+thumb.hash+"."+QFileInfo(thumb.name).suffix();
                    if(!thumb.image.save(QApplication::applicationDirPath()+"/.thumbs/"+thumb.hash+".png")){
                        qDebug()<< Q_FUNC_INFO<<"thumb konnte nicht gespeichert werden";
                    }else  {
                        mDatabaseHandler->insertHash( thumb.hash, mFolder, thumb.name, thumb.lastModi, thumb.size,"");
                    }
                }
        }
        mDatabaseHandler->finishTransaction();
    }
    //muhhhh
    if(!mEx)
        emit finish();

    qDebug() << Q_FUNC_INFO << timer.elapsed();
    deleteLater();
}
