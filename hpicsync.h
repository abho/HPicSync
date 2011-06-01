#ifndef HPICSYNC_H
#define HPICSYNC_H

#include <QtGui>
#include "hpsoption.h"
#include "hpsoptionwidget.h"
#include "hpslistviewdelegate.h"
#include "hpstcpmodul.h"
#include "hpsimageloader.h"
#include "hpsdirknoten.h"
#include "hpsdirmanager.h"
#include "hpstreecombobox.h"
#include <QtNetwork/QtNetwork>

class HPicSync : public QMainWindow
{
    Q_OBJECT
public:
    HPicSync(QWidget *parent = 0);
    ~HPicSync();
protected:
    void closeEvent(QCloseEvent *event);
private:
    QPushButton *mCloseButton,*mRefreshButton,*mOptionButton,*mtMarkAllButton,*mMarkSelectedButton,*mCopyButton,*mPlusButton;
    QListWidget *mNewListWidget,*mOldListWidget;
    QLabel *mPixOldLoadCountLabel,*mConnectPixRotLabel,*mConnectPixGruenLabel,*mConnectLabel;

  StandardHPSTreeCombobox *mTreeComboBox;
    QList<QImage> mThumbs;
    QMap<QThread *,HPSImageLoader *> mThreads;
    HPSOption mOption;
    HPSOptionWidget *mOptionWidget;
    HPSTCPModul *mTcpModul;
    HPSDirManager mDirManager;
    QElapsedTimer mTimer;
    bool mMoreThanOneSelected;
    int mPosImages;
    QMutex mMutex;

    void loadImages();
    void initCBOrdner(int index,const QString &dir);
private slots:
    void fotosReady(int size,const QString &str);
    void showOption();
    void test();
    void socketError(QAbstractSocket::SocketError error);
    void test2();
    void threadClear();
    void fertigTime();
    void clickedPlus();
    void comboBoxViewChanged(int);

};

#endif // HPICSYNC_H
