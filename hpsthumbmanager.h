#ifndef HPSTHUMBMANAGER_H
#define HPSTHUMBMANAGER_H

#include <QObject>
#include "hpsdbhandler.h"
#include <QListWidget>
#include <QMessageBox>
#include <QMainWindow>
#include <QApplication>
#include "hpsoption.h"
#include "hpsdirchecker.h"

class HPSThumbManager : public QObject
{
    Q_OBJECT
public:
    explicit HPSThumbManager( HPSOption &option, QObject *parent = 0);


    void setDatenBankHandler(HPSDBHandler *handler);
    void setListWidget(QListWidget *listWidget);
    void loadThumbs(const QString & dir);
    void startDirChecker();

signals:

public slots:

private:
    HPSOption &mOption;
    HPSDBHandler *mDatabaseHandler;
    HPSDirChecker *mDirChecker;
    QString mCurrentDir;
    QListWidget *mListWidget;
    QElapsedTimer mTimer;


    void makeView(const QList<QStringList> &list);


private slots:
     void fertigTime();
     void clearThread();


};

#endif // HPSTHUMBMANAGER_H
