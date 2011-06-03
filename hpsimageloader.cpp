#include "hpsimageloader.h"
#include <QElapsedTimer>
#include <QDebug>



QString HPSImageLoader::mFolder;
QVector<HPSThumb> *HPSImageLoader::mThumbVec;

HPSImageLoader::HPSImageLoader(QMutex &mutex, const int start,const int end, int size, QObject *parent) :
    HPSWorkerClass(parent),mSize(size),mStartPos(start),mEnd(end),mMutex(mutex)
{
}
void HPSImageLoader::startWithView(){
    qDebug() <<Q_FUNC_INFO << "start loader";
    load();
}

void HPSImageLoader::load(){
    int i;
    int lastSend = mStartPos-1;
    int nextSend = 0;
    QImageReader reader;
    //int length = size;
    int sizeW  = mSize-5;
    int sizeH =mSize-20;
    int counter=0;
    int packet=1;
    QSize imageSize;
    QImage image;
    QElapsedTimer timer;
    QByteArray block;
    QFile file;
    timer.start();
    nextSend = packet-1;

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

            // mMutex.lock();
            thumb.image = image.scaled( 200,200,Qt::KeepAspectRatio).
                    scaled(QSize(sizeW,sizeH),Qt::KeepAspectRatio,Qt::SmoothTransformation);


            // mMutex.unlock();

            if(counter==nextSend){
                qDebug() << thread()<<"ready" << i<<  lastSend+1<< packet;
                emit ready(lastSend+1,packet,"");
                lastSend =i;
                nextSend+=packet;
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
            if(i-1!=lastSend){
                qDebug() << "fehler ready" << lastSend+1 << i-lastSend-1;
                emit ready(lastSend+1,i-lastSend-1,"");

            }
            lastSend=i;
            nextSend=counter+packet;
        }
        counter++;
    }
    //qDebug() << "fertig: " << i;
    if(lastSend != i-1){
        qDebug() <<thread() <<  "ganz ready" << lastSend+1 << i-lastSend-1;
        emit ready(lastSend+1,i-lastSend-1,"");
    }
    deleteLater();
    // qDebug()<<timer.elapsed();
    emit fertig();
}
HPSImageLoader::~HPSImageLoader(){
    qDebug() << Q_FUNC_INFO<<"tot";
}







void HPSImageLoader::setFolder(const QString &folder)
{
    mFolder = folder;
}

void HPSImageLoader::startWithoutView()
{

    int i;
    QImageReader reader;
    QImage image;
    //int length = size;
    int sizeW  = mSize-5;
    int sizeH =mSize-20;
    int counter=0;
    QSize imageSize;
    QElapsedTimer timer;
    QByteArray block;
    QFile file;
    timer.start();
    //reader.setScaledSize(QSize(sizeW,sizeH));
    for(i  = mStartPos; i <= mEnd;i++){
        if(mEx){
            qDebug() <<"ex";
            deleteLater();
            return;
        }
        HPSThumb & thumb = (*mThumbVec)[i];
        //reader.setFileName(mFolder+"/"+mFileNames->at(i));

        file.setFileName(mFolder+"/"+thumb.name);

        //if(reader.read(&image)){
        if(file.open(QIODevice::ReadOnly))  {
            block =file.readAll();
            file.close();
            thumb.hash= QCryptographicHash::hash(block,QCryptographicHash::Sha1).toHex();

            image.loadFromData(block);
            image.setText(QString("name"),thumb.name);

            thumb.image=image.scaled( sizeW,sizeH,Qt::KeepAspectRatio).
                    scaled(QSize(sizeW,sizeH),Qt::KeepAspectRatio,Qt::SmoothTransformation);

        } else {
            thumb.error = true;
            qDebug() << "error" << i;
            emit error(i);
        }
        counter++;
    }
    deleteLater();
    // qDebug()<<timer.elapsed();
    emit fertig();
}

void HPSImageLoader::setThumbVector(QVector<HPSThumb> *thumbVec)
{
    mThumbVec = thumbVec;
}


