#ifndef HPSDIRLISTER_H
#define HPSDIRLISTER_H

#include <QObject>
#include <QtCore>
#include <QDebug>
#include "hpsoption.h"

class Saver
{
public:
    Saver(){}
    void add(const QString &str){
        const int size = str.size();
        QMutableListIterator<QString> i(mList);
        while(i.hasNext())
            if(i.next().left(size) ==str)
                i.remove();
        mList.append(str);
    }

    bool contains(const QString &str){
        return mList.contains(str);
    }
    const QStringList & list(){
        return mList;
    }
    void setList(const QStringList list){
        mList = list;
    }
private:
    QStringList mList;

};
class HPSDirLister : public QObject
{
    Q_OBJECT
public:
    explicit HPSDirLister(HPSOption &option, QStringList & oldList,const QStringList &saversFiles,const QString &path,QObject *parent = 0);
    ~HPSDirLister();
    QStringList dirs();
    QStringList saverDirs();


signals:
    void workDone();
    void workSemiDone();
    void chunkDone();
public slots:
    void startWork();
    void kill();
private:
    HPSOption &mOption;
    Saver mSaver;
    QString mPath;
    QStringList mList;
    const QStringList mOldList;
    QDir mDir;
    bool mEx;

    void subDirsFrom(const QString &dir);

};



#endif // HPSDIRLISTER_H
