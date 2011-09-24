#ifndef HPICSYNC_H
#define HPICSYNC_H

#include <QtGui>
#include "hpsoption.h"
#include "hpsoptionwidget.h"
#include "hpslistviewdelegate.h"
#include "hpstcpmodul.h"
#include "hpsimageloader.h"
#include "hpsdirmanager.h"
#include "hpstreecombobox.h"
#include "hpsdbhandler.h"
#include "hpsthumbmanager.h"
#include "hpsoldlistdelegate.h"
#include "hpsprogressbar.h"
#include "hpsdirdialog.h"
#include "hpscombobox.h"
#include "hpsthreadmanager.h"
#include  "ui_hpicsync.h"
#include "hpsdirwatcher.h"
#include <QtNetwork/QtNetwork>

class HPicSync : public QMainWindow
{
    Q_OBJECT
public:
    HPicSync(QWidget *parent = 0);
    QVector<QListWidgetItem*> & tmpListWidgetItems();
    ~HPicSync();
public slots:
    void onImageLoaderThumbsReady(int pos,int count);
    void onDirCheckerStartCheck(QString dir);
    void onDirCheckerFinished();
protected:
    void closeEvent(QCloseEvent *event);
signals:
    void startFirstRun();

private:

    Ui::HPicSync *ui;
    QLabel *mPixOldLoadCountLabel,*mConnectPixRotLabel,*mConnectPixGruenLabel,*mConnectLabel;


    HPSOption mOption;
    HPSOptionWidget *mOptionWidget;
    HPSTCPModul *mTcpModul;
    HPSThreadManager mThreadManager;
    HPSDBHandler mDatabaseHandler;
    HPSDirManager mDirManager;
    HPSThumbManager mThumbManager;

    HPSProgressBar *mBar;
    QElapsedTimer mTimer;
    QVector<QListWidgetItem*> mTmpListWidgetItems;
    bool mMoreThanOneSelected;
    int mPosImages;
    QMutex mMutex;


    void initCBOrdner(int index,const QString &dir);
    void initThumbManager();

private slots:
    void test();
    void socketError(QAbstractSocket::SocketError error);
    void test2();

    void comboBoxViewChanged(int);
    void comboBoxDirClicked( QString);

    void on_butOption_clicked();
    void on_butPlus_clicked();
    void onMoptionwidgetThumbsizechanged(int size);
    void on_butMinus_clicked();
};

#endif // HPICSYNC_H
