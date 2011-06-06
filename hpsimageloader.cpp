#include "hpsimageloader.h"
#include <QElapsedTimer>
#include <QDebug>



QString HPSImageLoader::mFolder;
QVector<HPSThumb> *HPSImageLoader::mThumbVec;

HPSImageLoader::HPSImageLoader(QMutex &mutex, const int start,const int end, int size, QObject *parent) :
    HPSWorkerClass(parent),mSize(size),mStartPos(start),mEnd(end),mMutex(mutex)
{
}

HPSImageLoader::~HPSImageLoader(){
    qDebug() << Q_FUNC_INFO<<"tot";
}
void HPSImageLoader::startWithView(){
    qDebug() <<Q_FUNC_INFO << "start loader";
    load(true);
}

void HPSImageLoader::load(bool withSignals){
    int i,packet,lastSend,nextSend;
    if(withSignals){
        packet=1;
        lastSend = mStartPos-1;
        nextSend = 0;
        nextSend = packet-1;
    }

    QImageReader reader;
    int sizeW  = mSize-5;
    int sizeH =mSize-20;
    int counter=0;
    QSize imageSize;
    QImage image;
    QByteArray block;
    QFile file;


    //reader.setScaledSize(QSize(sizeW,sizeH));
    for(i  = mStartPos; i <= mEnd;i++){
        if(mEx){
            qDebug() <<"ex";
            deleteLater();
            return;
        }
        HPSThumb &thumb = (*mThumbVec)[i];
        //reader.setFileName(mFolder+"/"+mFileNames->at(i));

        file.setFileName(mFolder+"/"+thumb.name);

        //if(reader.read(&image)){
        if(file.open(QIODevice::ReadOnly))  {
            block =file.readAll();
            file.close();
            thumb.hash = QCryptographicHash::hash(block,QCryptographicHash::Sha1).toHex();

            image.loadFromData(block);
            image.setText(QString("name"),QString(thumb.name));

            thumb.image = image.scaled( 200,200,Qt::KeepAspectRatio).
                    scaled(QSize(sizeW,sizeH),Qt::KeepAspectRatio,Qt::SmoothTransformation);

            if(withSignals){
                if(counter==nextSend){
                    qDebug() << thread()<<"ready" << i<<  lastSend+1<< packet;
                    emit ready(lastSend+1,packet);
                    lastSend =i;
                    nextSend+=packet;
                }
            }
        } else {
            /* mMutex.lock();
            int count;
            //qDebug() <<"fehler bei bild:" << ->dirName+->files.at(i);
            qDebug() << counter%packet;
            if(counter%packet!=0){
                // qDebug() << "fehler ready" << (i-(i%5)) << (i-(i-(i%5)));
                 qDebug()<<"i"<< i;
                count=counter%packet;
           } else{
                count =0;
            }
            if(mSendError ==false ){
                qDebug() << count;
                emit ready(i-count,count,mFileNames->at(i));
            }else{
                emit ready(i-count,count,"");
}
            deleteLater();
            mSendError=true;
            mMutex.unlock();
            return;*/
            qDebug() << "error" << i;
            emit error(i);
            thumb.error = true;
            if(withSignals){
                if(i-1!=lastSend){
                    qDebug() << "fehler ready" << lastSend+1 << i-lastSend-1;
                    emit ready(lastSend+1,i-lastSend-1);

                }
                lastSend=i;
                nextSend=counter+packet;
            }
        }
        counter++;
        fotosReady();
    }
    //qDebug() << "fertig: " << i;
    if(withSignals){
        if(lastSend != i-1){
            qDebug() <<thread() <<  "ganz ready" << lastSend+1 << i-lastSend-1;
            emit ready(lastSend+1,i-lastSend-1);
        }
    }
    deleteLater();
    emit fertig();
}










void HPSImageLoader::setFolder(const QString &folder)
{
    mFolder = folder;
}

void HPSImageLoader::startWithoutView()
{
    load(false);

}

void HPSImageLoader::setThumbVector(QVector<HPSThumb> *thumbVec)
{
    mThumbVec = thumbVec;
}


