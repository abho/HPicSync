#ifndef HPSIMAGELOADER_H
#define HPSIMAGELOADER_H

#include <QObject>
#include <QImageReader>
#include <QtCore>
#include <QtAlgorithms>
//muh
class HPSImageLoader : public QObject
{
    Q_OBJECT
public:
    explicit HPSImageLoader(QMutex &mutex,const int start,const int end,const int size,QObject *parent = 0);
    ~HPSImageLoader();
    void beenden();
    static void setImageVector(QVector<QImage> & imageVec);
    static void setHashVector(QVector<QString> & hashVec);
    static void setFileNames(const QStringList &fileNames);
    static void setFolder(const QString &folder);

private:
    const QString mQuellOrdner;

    const int mSize,mStartPos,mEnd;
    QMutex &mMutex;
    bool mEx;

    static bool mSendError;
    static QStringList const mFileNames;
    static QVector<QImage> &mImageVec;
    static QVector<QImage> &mHashVec;
    static const QString mFolder;
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
