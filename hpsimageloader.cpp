#include "hpsimageloader.h"
#include <QElapsedTimer>
#include <QDebug>



QString HPSImageLoader::mFolder;
QVector<HPSThumb> *HPSImageLoader::mThumbVec;

HPSImageLoader::HPSImageLoader(  QObject *parent) :
    QObject(parent),mIsRunning(false),mShutDown(false)
{
}

HPSImageLoader::~HPSImageLoader(){
    qDebug() << Q_FUNC_INFO<<"tot";
}

void HPSImageLoader::setFolder(const QString &folder)
{
    mFolder = folder;
}
void HPSImageLoader::setThumbVector(QVector<HPSThumb> *thumbVec)
{
    mThumbVec = thumbVec;
}

void HPSImageLoader::startWork()
{
    qDebug() << Q_FUNC_INFO << thread();
    mIsRunning = true;

     QElapsedTimer timer;
    int i,packet,lastSend,nextSend;
    if(mWithView){
        packet=1;
        lastSend = mStartPos-1;
        nextSend = 0;
        nextSend = packet-1;
    }
    int image_width;
    int image_height;
    QImageReader reader;
    int sizeW  = mSize-4;
    int sizeH =mSize-20;
    int counter=0;
    QSize imageSize;
    QImage image;
    QByteArray block;
    QFile file;
    timer.start();

    for(i  = mStartPos; i <= mEndPos;i++){
        if(mShutDown){
            break;
        }
        //qDebug() <<QThread::currentThreadId()<< "start" <<timer.elapsed();
        HPSThumb &thumb = (*mThumbVec)[i];

        reader.setFileName(mFolder+"/"+thumb.name);
        image_width = reader.size().width();
        image_height = reader.size().height();
        if (image_width > image_height) {
            image_height = static_cast<double>(sizeW) / image_width * image_height;
            image_width = sizeW;
        } else if (image_width < image_height) {
            image_width = static_cast<double>(sizeH) / image_height * image_width;
            image_height = sizeH;
        } else {
            image_width = sizeW;
            image_height = sizeH;
        }
        reader.setScaledSize(QSize(image_width, image_height));
        thumb.image =  reader.read();
         //qDebug() <<QThread::currentThreadId()<< "readerload" <<timer.elapsed();

         file.setFileName(mFolder+"/"+thumb.name);
        if(file.open(QIODevice::ReadOnly)&&!thumb.image.isNull())  {
            block =file.readAll();
            //qDebug() << QThread::currentThreadId()<<"open" <<timer.elapsed();
            file.close();
            thumb.hash = QCryptographicHash::hash(block,QCryptographicHash::Md5).toHex();
            //qDebug() << QThread::currentThreadId()<<"hash" <<timer.elapsed();
    /*
            image.loadFromData(block);

qDebug() <<QThread::currentThreadId()<< "scale" <<timer.elapsed();
            thumb.image = image.scaled( 200,200,Qt::KeepAspectRatio).
                  scaled(QSize(sizeW,sizeH),Qt::KeepAspectRatio,Qt::SmoothTransformation);

qDebug() <<QThread::currentThreadId()<< "afterscale" <<timer.elapsed();
*/
            if(mWithView){
                if(counter==nextSend){
            //        qDebug() << thread()<<"ready" << i<<  lastSend+1<< packet;
                    emit ready(lastSend+1,packet);
                    lastSend =i;
                    nextSend+=packet;
                }
            }
        } else {
            //qDebug() << "error" << i;
            emit error(i);
            thumb.error = true;
            if(mWithView){
                if(i-1!=lastSend){
                  //  qDebug() << "fehler ready" << lastSend+1 << i-lastSend-1;
                    emit ready(lastSend+1,i-lastSend-1);
                }
                lastSend=i;
                nextSend=counter+packet;
            }
        }
        counter++;
    }
    //qDebug() << "fertig: " << i;
    if(mWithView){
        if(lastSend != i-1){
            //qDebug() <<thread() <<  "ganz ready" << lastSend+1 << i-lastSend-1;
            emit ready(lastSend+1,i-lastSend-1);
        }
    }
    if(mShutDown)
        deleteLater();
    else{
        emit fertig();
        mIsRunning = false;
    }
}

void HPSImageLoader::close()
{
    if(mIsRunning)
        mShutDown = true;
    else
        deleteLater();
}

void HPSImageLoader::setWork(const int start, const int end, int size, bool withView)
{
    mSize = size;
    mStartPos = start;
    mEndPos = end;
    mWithView = withView;
}

const QString & HPSImageLoader::folder()
{
    return mFolder;
}


