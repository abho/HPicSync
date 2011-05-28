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
    QPushButton *butClose,*butRefresh,*butOption,*butMarkAll,*butMarkSelected,*butCopy,*butPlus;
    QListWidget *newList,*oldList;
    QLabel *lPixOldLoadCount,*lConnectPixRot,*lConnectPixGruen,*lConnect;
    //QComboBox *coOrdner;
  Standardhpstreecombobox *coOrdner;
  QTreeWidget *tree;
  //  HTreeWidget *tree;
    QList<QImage> thumbs;
    QMap<QThread *,HPSImageLoader *> threads;
    HPSOption option;
    HPSOptionWidget *optionWidget;
    HPSTCPModul *tcpModul;
    HPSDirManager dirManager;
    QTcpSocket *socket;
    QElapsedTimer timer;
    bool moreThanOneSelected;
    int posImages;
    QMutex mutex;

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
    //void dirListChanged();

};

#endif // HPICSYNC_H
