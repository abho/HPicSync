#ifndef HPSIMAGELOADER_H
#define HPSIMAGELOADER_H

#include <QObject>
#include <QImageReader>
#include <QtCore>
#include <QtAlgorithms>
#include "hpsworkerclass.h"
#include "hpsthumb.h"
//muh
class HPSImageLoader : public HPSWorkerClass
{
    Q_OBJECT
public:
    explicit HPSImageLoader(QMutex &mutex,const int startWithView,const int end,const int size,QObject *parent = 0);
    ~HPSImageLoader();

    static void setThumbVector(QVector<HPSThumb> * thumbVec);
    static void setFolder(const QString &folder);

private:

    const int mSize,mStartPos,mEnd;
    QMutex &mMutex;


    static QVector<HPSThumb> *mThumbVec;
    static QString mFolder;

signals:
    void fotosReady();
    void error(const int errorPos);
    void fertig();
    void ready(int pos,int count);

public slots:
    void startWithView();
    void startWithoutView();
private slots:
    void load(bool withSignals);
};

#endif // HPSIMAGELOADER_H
