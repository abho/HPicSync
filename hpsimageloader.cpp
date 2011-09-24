#include "hpsimageloader.h"
#include <QElapsedTimer>
#include <QDebug>





HPSImageLoader::HPSImageLoader(QVector<HPSThumb> &thumbVec,  QObject *parent) :
    QObject(parent),mThumbVec(thumbVec),mIsRunning(false),mShutDown(false)
{
}
HPSImageLoader::~HPSImageLoader(){
    qDebug() << Q_FUNC_INFO << "tot";
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

        HPSThumb &thumb = mThumbVec[i];
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
       if(image_width == 0)
           image_width =1;
       if(image_height == 0)
           image_height =1;
        reader.setScaledSize(QSize(image_width, image_height));
        thumb.image =  reader.read();
         file.setFileName(mFolder+"/"+thumb.name);
        if(file.open(QIODevice::ReadOnly)&&!thumb.image.isNull())  {
            block =file.readAll();
            file.close();
            thumb.hash = QCryptographicHash::hash(block,QCryptographicHash::Md5).toHex();

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
            file.close();
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

void HPSImageLoader::setWork(const QString &folder ,const int start, const int end, int size, bool withView)
{
    mFolder = folder;
    mSize = size;
    mStartPos = start;
    mEndPos = end;
    mWithView = withView;
}

const QString & HPSImageLoader::folder()
{
    return mFolder;
}




