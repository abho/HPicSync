#ifndef HPSIMAGELOADER_H
#define HPSIMAGELOADER_H

#include <QObject>
#include <QImageReader>
#include <QtCore>

class HPSImageLoader : public QObject
{
    Q_OBJECT
public:
    explicit HPSImageLoader(QMutex &mutex,const QStringList &fileNames,const int start,const int end,const QString &quellOrdner,int size,QList<QImage>*thumbs,QObject *parent = 0);
    ~HPSImageLoader();
    void beenden();
private:
    const QString &quellOrdner;
    QList<QImage> *list;
    const int size,startPos,end;
    QStringList fileNames;
    QMutex &mutex;
    bool ex;
   static bool sendError;
    signals:
        void fotosReady(int pos, int count);
        void error(int pos,int count,const QString&str);
        void fertig();
        void ready(int size,const QString &str);

    public slots:
        void start();
    private slots:
        void load();
};

#endif // HPSIMAGELOADER_H
