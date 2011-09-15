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
    explicit HPSImageLoader(QObject *parent = 0);
    ~HPSImageLoader();

    static void setThumbVector(QVector<HPSThumb> * thumbVec);
    static void setFolder(const QString &folder);

private:
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
    void start(const int start,const int end, int size,bool withView);
private slots:
    void load(bool withSignals);
};

#endif // HPSIMAGELOADER_H
