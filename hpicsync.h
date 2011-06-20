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
    QPushButton *mCloseButton,*mRefreshButton,*mOptionButton,*mtMarkAllButton,*mMarkSelectedButton,*mCopyButton,*mPlusButton,*mMinusButton;
    QListWidget *mNewListWidget,*mOldListWidget;
    QLabel *mPixOldLoadCountLabel,*mConnectPixRotLabel,*mConnectPixGruenLabel,*mConnectLabel;

  HPSTreeCombobox *mTreeComboBox;


    HPSOption mOption;
    HPSOptionWidget *mOptionWidget;
    HPSTCPModul *mTcpModul;
    HPSDirManager mDirManager;
    HPSDBHandler mDatabaseHandler;
    HPSThumbManager mThumbManager;
    HPSProgressBar *mBar;
    QElapsedTimer mTimer;
    bool mMoreThanOneSelected;
    int mPosImages;
    QMutex mMutex;

    void loadImages(const QString &folder);
    void initCBOrdner(int index,const QString &dir);
    void initThumbManager();
private slots:
    void showOption();
    void test();
    void socketError(QAbstractSocket::SocketError error);
    void test2();
    void refreshBar(int value);
    void initBar(const QString &,const int);
    void clickedPlus();
    void comboBoxViewChanged(int);
    void ordnerRemoved(QStringList dirs);
    void comboBoxItemclicked( QModelIndex );
    void clickedMinus();
    void startBar();
    void finishBar();


};

#endif // HPICSYNC_H
