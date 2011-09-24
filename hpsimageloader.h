#ifndef HPSIMAGELOADER_H
#define HPSIMAGELOADER_H

#include <QObject>
#include <QImageReader>
#include <QtCore>
#include <QtAlgorithms>
#include "hpsthumb.h"

class HPSImageLoader : public QObject
{
    Q_OBJECT
public:
    explicit HPSImageLoader(QVector<HPSThumb> &thumbVec,QObject *parent = 0);
    ~HPSImageLoader();

    //static void setThumbVector(QVector<HPSThumb> * thumbVec);
    //static void setFolder(const QString &folder);
   const QString &folder();
    void setWork(const QString &folder,const int start,const int end, int size,bool withView);
    void close();

signals:
    void error(const int errorPos);
    void fertig();
    void ready(int pos,int count);

public slots:
    void startWork();

private:
    QVector<HPSThumb> &mThumbVec;
    QString mFolder;
    int mStartPos,mEndPos,mSize;
    bool mWithView;
    bool mIsRunning;
    bool mShutDown;

};

#endif // HPSIMAGELOADER_H
