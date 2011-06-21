#include "hpicsync.h"
#include <QDebug>

HPicSync::HPicSync(QWidget *parent)
    : QMainWindow(parent),mOptionWidget(NULL),mDirManager(mOption),mThumbManager( mOption),mMoreThanOneSelected(false)
{


    QHBoxLayout *optionBox = new QHBoxLayout();
    this->mOptionButton = new QPushButton(tr("Option"));
    optionBox->addStretch();
    optionBox->addWidget(this->mOptionButton);

    QHBoxLayout *viewBox= new QHBoxLayout();

    QVBoxLayout *newListBox= new QVBoxLayout();
    QHBoxLayout *refreshBox = new QHBoxLayout();
    this->mRefreshButton = new QPushButton(tr("Refresh"));
    refreshBox->addWidget(mRefreshButton);
    refreshBox->addStretch();

    this->mNewListWidget = new QListWidget();
    this->mNewListWidget->setFlow(QListView::LeftToRight);
    this->mNewListWidget->setWrapping(true);
    this->mNewListWidget->setUniformItemSizes(true);
    this->mNewListWidget->setResizeMode(QListView::Adjust);
    this->mNewListWidget->setAutoScroll(false);
    this->mNewListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    HPSListViewDelegate *delegateNew = new HPSListViewDelegate(this->mOption.getThumbSize(),&this->mMoreThanOneSelected,this);
    this->mNewListWidget->setItemDelegate(delegateNew);


    QHBoxLayout * buttonBox = new QHBoxLayout();
    this->mtMarkAllButton = new QPushButton(trUtf8("Alle auswählen"));
    this->mMarkSelectedButton = new QPushButton(trUtf8("Markierte auswählen"));
    buttonBox->addWidget(this->mtMarkAllButton);
    buttonBox->addWidget(this->mMarkSelectedButton);
    buttonBox->addStretch();

    newListBox->addLayout(refreshBox);
    newListBox->addWidget(mNewListWidget);
    newListBox->addLayout(buttonBox);

    this->mCopyButton = new QPushButton(tr("copy"));

    QVBoxLayout *oldListBox = new QVBoxLayout();


    QHBoxLayout *comboBox = new QHBoxLayout();

    this->mTreeComboBox = new HPSTreeCombobox(this);
    mDirManager.setModel( mTreeComboBox->standardModel());
    initCBOrdner(mOption.getComboBoxView(),mOption.getComboBoxCurrentDir());
    mPlusButton=new QPushButton(tr("plus"));
    mMinusButton = new QPushButton( trUtf8("minus"));
    comboBox->addWidget(mTreeComboBox,10);
    comboBox->addWidget(mPlusButton,1);
    comboBox->addWidget(mMinusButton,1);

    this->mOldListWidget = new QListWidget();
    this->mOldListWidget->setFlow(QListView::LeftToRight);
    this->mOldListWidget->setWrapping(true);
    this->mOldListWidget->setUniformItemSizes(true);
    this->mOldListWidget->setResizeMode(QListView::Adjust);
    this->mOldListWidget->setAutoScroll(false);
    this->mOldListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    HPSOldListDelegate *delegateOld = new HPSOldListDelegate(this->mOption.getThumbSize(),this);
    this->mOldListWidget->setItemDelegate(delegateOld);

    QHBoxLayout *beendenBox = new QHBoxLayout();
    this->mCloseButton = new QPushButton(tr("Beenden"));
    beendenBox->addStretch();
    beendenBox->addWidget(this->mCloseButton);

    oldListBox->addLayout(comboBox);
    oldListBox->addWidget(mOldListWidget);
    oldListBox->addLayout(beendenBox);
    viewBox->addLayout(newListBox);
    viewBox->addWidget(this->mCopyButton);
    viewBox->addLayout(oldListBox);

    QVBoxLayout *mainBox = new QVBoxLayout();
    mainBox->addLayout(optionBox);
    mainBox->addLayout(viewBox);

    QWidget *mainWidget = new QWidget();
    mainWidget->setLayout(mainBox);
    this->setCentralWidget(mainWidget);

    QStatusBar *bar = new QStatusBar();
    this->setStatusBar(bar);
    this->mConnectLabel = new QLabel(tr("nicht verbunden"));
    this->mConnectPixGruenLabel = new QLabel();
    this->mConnectPixGruenLabel->setPixmap(QPixmap(":/knopfGruen").scaled(QSize(17,17),Qt::KeepAspectRatio));
    this->mConnectPixRotLabel = new QLabel();
    this->mConnectPixRotLabel->setPixmap(QPixmap(":/knopfRot").scaled(QSize(17,17),Qt::KeepAspectRatio));
    mBar = new HPSProgressBar;
    // this->mPixOldLoadCountLabel = new QLabel("");
    //this->mPixOldLoadCountLabel->setVisible(false);
    mBar->setVisible(false);
    mBar->setValue(0);
    mBar->setTextVisible(false);
    this->statusBar()->addWidget(this->mConnectPixGruenLabel);
    this->statusBar()->addWidget(this->mConnectPixRotLabel);
    this->statusBar()->addWidget(this->mConnectLabel);
    this->statusBar()->addPermanentWidget( mBar);

    this->connect(this->mCloseButton,SIGNAL(clicked()),this,SLOT(close()));
    this->connect(this->mOptionButton,SIGNAL(clicked()),this,SLOT(showOption()));
    this->connect(this->mRefreshButton,SIGNAL(clicked()),this,SLOT(test()));
    this->connect(this->mCopyButton,SIGNAL(clicked()),this,SLOT(test2()));
    connect( mTreeComboBox,SIGNAL(itemClicked(QModelIndex)),this,SLOT(comboBoxItemclicked(QModelIndex)));
    connect(mPlusButton,SIGNAL(clicked()),this,SLOT(clickedPlus()));
    connect( &mThumbManager,SIGNAL(thumbsReady(int)),this,SLOT(refreshBar(int)));
    connect( &mThumbManager,SIGNAL(startThumbCreation(QString,int)),this,SLOT(initBar(QString,int)));
    connect( &mThumbManager,SIGNAL(startCreation()),this,SLOT(startBar()));
    connect( &mThumbManager,SIGNAL(creationReady()),this,SLOT(finishBar()));
    connect( mMinusButton,SIGNAL(clicked()),this,SLOT(clickedMinus()));
    this->setGeometry(this->mOption.getGeometry());

    if(!mDatabaseHandler.openDatabase("picsync.db"))
        QMessageBox::critical(this, trUtf8("Fehler"), trUtf8("Verbindeung mit der Datenbank konnte nicht hergestellt werden."),QMessageBox::Ok);

    QDir dir( QApplication::applicationDirPath());
    if(!dir.exists(".thumbs")){
        dir.mkdir(".thumbs");
    }
    initThumbManager();


}
HPicSync::~HPicSync()
{
    mOption.setGeometry(this->geometry());
    mOption.setComboBoxCurrentDir(mTreeComboBox->currentDir());
    mOption.setExpandDirs( mTreeComboBox->expandeDirs());
}





void HPicSync::closeEvent(QCloseEvent *event){
    qDebug() << "closeEvent" << mThumbManager.allThreadsClose();
    if(mThumbManager.allThreadsClose()){
        event->accept();
    } else {
        mThumbManager.closeAllThreads();
        connect(&mThumbManager,SIGNAL(allThreadsDestroyed()),this,SLOT(close()));
        event->ignore();
    }

}



void HPicSync::showOption(){
    if(this->mOptionWidget== NULL){
        this->mOptionWidget = new HPSOptionWidget(&this->mOption,this);
        connect(mOptionWidget,SIGNAL(comboBoxViewSelectedChanged(int)),this,SLOT(comboBoxViewChanged(int)));
        //connect( mOptionWidget,SIGNAL(dirsRemoved(QStringList)), &mDirManager,SLOT(removeDirs(QStringList)));
        connect( mOptionWidget,SIGNAL(dirsRemoved(QStringList)),this,SLOT(ordnerRemoved(QStringList)));
        qDebug() << "optionWidget is null";
    }
    this->mOptionWidget->resetAndShow();
}
void HPicSync::test(){
    /* qDebug()<< "start socket";
    this->tcpModul = new HPSTCPModul(&this->option,this);
    this->tcpModul->startSearch();
*/
    // qDebug() << " start loader";
    //this->loadImages();
    //this->dirManager.showTree();
    //this->coOrdner->setViewToList();
    //this->dirManager.makeListView(this->coOrdner->model());

    // this->dirManager.makeTreeView(this->coOrdner->model());
    /*QFile file("muh.jpg");
    if(file.open(QIODevice::ReadOnly)){
        QElapsedTimer timer;
        QByteArray block = file.readAll();
        timer.start();
        QCryptographicHash::hash(block,QCryptographicHash::Sha1);
        qDebug() << timer.elapsed();
        timer.restart();
        QCryptographicHash::hash(block,QCryptographicHash::Md5);
        qDebug() << timer.elapsed();
    }else {
        qDebug() << "fehler"<< file.errorString();
    }
*/
    //loadImages("C:/Users/hakah/me");
    // loadImages("/home/hakah/Dokumente/HFotoCrapper-build-desktop/image");
    /*QFile file("C:/Users/hakah/me/fastfertig.jpg");
    if(file.open(QIODevice::ReadOnly))
        qDebug() << true;
    else
        qDebug()<< false;
*/
    mDirManager.reset();



}
void HPicSync::loadImages(const QString &folder){
    /*qDebug() << Q_FUNC_INFO;

    const int size = mThumbManager.creatThumbs(folder,true,true);
    if(size != 0){
        mBar->setVisible(true);
        mBar->setMaxCount(size);
        mBar->setCount(0);
        mBar->setCountVisible(true);
    }*/
    mThumbManager.creatThumbs(folder,true);
}



void HPicSync::socketError(QAbstractSocket::SocketError error){
    qDebug() << "error "<< error;
}
void HPicSync::test2() {
    // qDebug() << this->socket->state();


}



void HPicSync::comboBoxViewChanged(int index)
{
    initCBOrdner(index,mTreeComboBox->currentDir());

}

void HPicSync::clickedPlus()
{
    const QString str = QDir::fromNativeSeparators(QFileDialog::getExistingDirectory(this,QDir::homePath()));
    //(qDebug() << str;
    if(!str.isEmpty()){
        QMessageBox dialog(this);
        dialog.setText( trUtf8("Unterordner auch hinzuf�gen?"));
        QPushButton *jaButton = dialog.addButton( trUtf8("Ja"),QMessageBox::YesRole);
        QPushButton *neinButton =  dialog.addButton(trUtf8("Nein"),QMessageBox::NoRole);
        QPushButton *abbruchButton = dialog.addButton(trUtf8("Abbruch"),QMessageBox::RejectRole);
        dialog.exec();
        qDebug() <<Q_FUNC_INFO<< dialog.clickedButton();
        if(dialog.clickedButton() == abbruchButton){
            qDebug() << "abbruch";
            return ;
        } else if(dialog.clickedButton() == jaButton) {
            qDebug() << "ja";
           mDirManager.addDir(str,true);
        } else if(dialog.clickedButton() == neinButton) {
            qDebug() << "nein";
            mDirManager.addDir(str,false);
        }
       // mTreeComboBox->findeAndSetCurrentItem(str);

    }
}
/* HPSDirDialog *dialog = new HPSDirDialog(this);
    dialog->show();
*/


void HPicSync::initCBOrdner(int index,const QString &dir)
{
    //qDebug() << "initCBOrdner" << index << dir;
    if(index == HPSOption::ListView){
        mOption.setExpandDirs(mTreeComboBox->expandeDirs());
        mTreeComboBox->setViewToList();
    } else {
        mTreeComboBox->setViewToTree();
    }
    //QList<QStandardItem*> expandesIems= mDirManager.makeView();
    //if(!expandesIems.isEmpty())
    //  mTreeComboBox->setExpandedItems( expandesIems );
    //mTreeComboBox->findeAndSetCurrentItem(dir);
}

void HPicSync::comboBoxItemclicked(QModelIndex index)
{
    QString path = index.data(Qt::UserRole).toString();
    qDebug() << Q_FUNC_INFO << path;
    if(path != QDir::fromNativeSeparators(mTreeComboBox->currentText())){
        qDebug() << "neues";
        if(mThumbManager.dirReady(path)){
            mThumbManager.loadThumbs(path);
        }else{
            //anpassen
        }

    } else {
        qDebug() << "currentItem clicked";
    }

}

void HPicSync::ordnerRemoved(QStringList dirs)
{

  /*  if(dirs.contains( mTreeComboBox->currentDir()))
        mTreeComboBox->setCurrentItem(NULL);

    mDirManager.removeDirs(dirs);
*/
}


void HPicSync::refreshBar(int value)
{
    qDebug() << Q_FUNC_INFO << value;
    mBar->setValue(value);
}

void HPicSync::initBar(const QString &dir, const int size)
{
    qDebug() << Q_FUNC_INFO << dir << size;

    mBar->setFormat("creating thumbnails: "+dir+"...("+QString::number(mThumbManager.workCount())+")");
    if(size == 0){
        mBar->setRange(0,1);
        mBar->setValue(1);
    }else {
        mBar->setRange(0,size);
        mBar->setValue(0);
    }

}

void HPicSync::initThumbManager()
{
    mThumbManager.setDatenBankHandler( &mDatabaseHandler);
    mThumbManager.setListWidget( mOldListWidget);
    mThumbManager.loadThumbs( mTreeComboBox->currentDir());
    bool workToDo = mThumbManager.startWork();
    qDebug() << Q_FUNC_INFO << workToDo;
}

void HPicSync::clickedMinus()
{
    /*
    QMessageBox dialog(this);
    dialog.setText( trUtf8("Unterordner mitentfernen?"));
    QPushButton *jaButton = dialog.addButton( trUtf8("Ja"),QMessageBox::YesRole);
    QPushButton *neinButton =  dialog.addButton(trUtf8("Nein"),QMessageBox::NoRole);
    QPushButton *abbruchButton = dialog.addButton(trUtf8("Abbruch"),QMessageBox::RejectRole);
    dialog.exec();
    if(dialog.clickedButton() == abbruchButton){
        qDebug() << "abbruch";
        return ;
    } else if(dialog.clickedButton() == jaButton) {
        qDebug() << "ja";
        mDirManager.removeDir( QDir::fromNativeSeparators( mTreeComboBox->currentText()),true);
    } else if(dialog.clickedButton() == neinButton) {
        qDebug() << "nein";
        mDirManager.removeDir( QDir::fromNativeSeparators( mTreeComboBox->currentText()),false);
    }
    mTreeComboBox->updateText();
    */
}

void HPicSync::startBar()
{
    mBar->setVisible(true);
}

void HPicSync::finishBar()
{
    mBar->setVisible(false);
}
