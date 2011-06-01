#ifndef HPSIMAGELOADER_H
#define HPSIMAGELOADER_H

#include <QObject>
#include <QImageReader>
#include <QtCore>
//muh
class HPSImageLoader : public QObject
{
    Q_OBJECT
public:
    explicit HPSImageLoader(QMutex &mutex,const QStringList &fileNames,const int start,const int end,const QString &quellOrdner,int size,QList<QImage>*thumbs,QObject *parent = 0);
    ~HPSImageLoader();
    void beenden();
private:
    const QString &mQuellOrdner;
    QList<QImage> *mList;
    const int mSize,mStartPos,mEnd;
    QStringList mFileNames;
    QMutex &mMutex;
    bool mEx;
   static bool mSendError;
    signals:
        void fotosReady(int pos, int count);
        void error(int pos,int count,const QString&str);
        void fertig();
        void ready(int mSize,const QString &str);

    public slots:
        void start();
    private slots:
        void load();
};

#endif // HPSIMAGELOADER_H
