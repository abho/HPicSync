#include "hpsimageloader.h"
#include <QElapsedTimer>
#include <QDebug>
 bool HPSImageLoader::mSendError =false;
HPSImageLoader::HPSImageLoader(QMutex &mutex,const QStringList&fileNames, const int start,const int end,const QString &quellOrdner, int size, QList<QImage> *thumbs, QObject *parent) :
        QObject(parent),mMutex(mutex),mQuellOrdner(quellOrdner),mList(thumbs),mSize(size),mStartPos(start),mEnd(end),
        mFileNames(fileNames),mEx(false)
{
}
void HPSImageLoader::start(){
    qDebug() << "start loader";

    load();
}

void HPSImageLoader::load(){
    int i;
    QImageReader reader;
    QImage image;
    //int length = size;
    int sizeW  = mSize-5;
    int sizeH =mSize-20;
    int counter=0;
    int packet=10;
    QSize imageSize;
    QElapsedTimer timer;
    timer.start();
    //reader.setScaledSize(QSize(sizeW,sizeH));
    for(i  = mStartPos; i <= mEnd;i++){
        if(mEx){
            qDebug() <<"ex";
            deleteLater();
            return;
        }
        reader.setFileName(mQuellOrdner+"/"+mFileNames.at(i));

        //imageSize = reader.size();
        //imageSize.scale(QSize(sizeW,sizeH), Qt::KeepAspectRatio);
        //reader.setScaledSize(imageSize);
        /*        image_width = reader.size().width();
         image_height =reader.size().height();
        if (image_width > image_height) {
          image_height = static_cast<double>(length-5) / image_width * image_height;
          image_width = length-5;
        } else if (image_width < image_height) {
          image_width = static_cast<double>(length-20) / image_height * image_width;
          image_height = length-20;
        } else {
          image_width = length-5;
          image_height = length-20;
        }
        */
        //reader.setScaledSize(QSize(sizeW,sizeH));
        if(reader.read(&image)){
            image =image.scaled(200,200,Qt::KeepAspectRatio).
                   scaled(QSize(sizeW,sizeH),Qt::KeepAspectRatio,Qt::SmoothTransformation);
            image.setText(QString("name"),QString(mFileNames.at(i)));
            mMutex.lock();
            mList->append(image);
            mMutex.unlock();

            if((counter+1)%packet==0&&(counter!=0)){
                qDebug() << thread()<<"ready" << packet << counter;
                ready(packet,"");
            }
        } else {
            mMutex.lock();
            int count;
            //qDebug() <<"fehler bei bild:" << ->dirName+->files.at(i);
            if((counter)%packet!=0)
                // qDebug() << "fehler ready" << (i-(i%5)) << (i-(i-(i%5)));
                // qDebug()<<"i"<< i;
                count=counter&packet;
            else
                count =0;
            if(mSendError ==false )
                emit ready(count,mFileNames.at(i));
            else
                emit ready(count,"");

            deleteLater();
            mSendError=true;
            mMutex.unlock();
            return;
        }

        counter++;

    }
    //qDebug() << "fertig: " << i;
    if(counter%packet!=0){
        //qDebug() <<thread() <<  "ready" << counter%5 << counter;
        emit ready(counter%packet,"");
    }
    deleteLater();
    // qDebug()<<timer.elapsed();
    emit fertig();
}
void HPSImageLoader::beenden() {
    qDebug()<<"beeenden";
    mEx=true;

}

HPSImageLoader::~HPSImageLoader(){
    qDebug()<<"tot";
}
