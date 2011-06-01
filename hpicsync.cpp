#include "hpicsync.h"
#include <QDebug>

HPicSync::HPicSync(QWidget *parent)
    : QMainWindow(parent),mOptionWidget(NULL),mDirManager(mOption),mMoreThanOneSelected(false)
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

    this->mTreeComboBox = new StandardHPSTreeCombobox(this);
    mDirManager.setModel( mTreeComboBox->model());
    initCBOrdner(mOption.getComboBoxView(),mOption.getComboBoxCurrentDir());
    mPlusButton=new QPushButton(tr("plus"));
    comboBox->addWidget(mTreeComboBox,10);
    comboBox->addWidget(mPlusButton,1);

    this->mOldListWidget = new QListWidget();
    this->mOldListWidget->setFlow(QListView::LeftToRight);
    this->mOldListWidget->setWrapping(true);
    this->mOldListWidget->setUniformItemSizes(true);
    this->mOldListWidget->setResizeMode(QListView::Adjust);
    this->mOldListWidget->setAutoScroll(false);
    this->mOldListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    HPSListViewDelegate *delegateOld = new HPSListViewDelegate(this->mOption.getThumbSize(),&this->mMoreThanOneSelected,this);
    this->mOldListWidget->setItemDelegate(delegateOld);

    QHBoxLayout *beendenBox = new QHBoxLayout();
    this->mCloseButton = new QPushButton(tr("Beenden"));
    beendenBox->addStretch();
    beendenBox->addWidget(this->mCloseButton);

    oldListBox->addLayout(comboBox);
    oldListBox->addWidget(mOldListWidget);
    qDebug() << mPlusButton->font().pointSize();
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
    this->mPixOldLoadCountLabel = new QLabel("");
    this->mPixOldLoadCountLabel->setVisible(false);

    this->statusBar()->addWidget(this->mConnectPixGruenLabel);
    this->statusBar()->addWidget(this->mConnectPixRotLabel);
    this->statusBar()->addWidget(this->mConnectLabel);
    this->statusBar()->addPermanentWidget(this->mPixOldLoadCountLabel);

    this->connect(this->mCloseButton,SIGNAL(clicked()),this,SLOT(close()));
    this->connect(this->mOptionButton,SIGNAL(clicked()),this,SLOT(showOption()));
    this->connect(this->mRefreshButton,SIGNAL(clicked()),this,SLOT(test()));
    this->connect(this->mCopyButton,SIGNAL(clicked()),this,SLOT(test2()));
    connect(mPlusButton,SIGNAL(clicked()),this,SLOT(clickedPlus()));

    this->setGeometry(this->mOption.getGeometry());

}
HPicSync::~HPicSync()
{
    mOption.setGeometry(this->geometry());
    mOption.setComboBoxCurrentDir(mTreeComboBox->getCurrentDir());
    mOption.setExpandDirs( mTreeComboBox->expandeDirs());
}





void HPicSync::closeEvent(QCloseEvent *event){
    qDebug() << "closeEvent" << this->mThreads;
    if(this->mThreads.isEmpty()){
        event->accept();
    } else {
        QMapIterator<QThread *, HPSImageLoader*> i(this->mThreads);
        while (i.hasNext()) {
            i.next();
            this->connect(i.key(),SIGNAL(destroyed()),this,SLOT(close()));
            i.value()->beenden();
        }
        event->ignore();
    }
}
void HPicSync::threadClear() {
    QThread *thread =(QThread*)sender();
    this->mThreads.remove(thread);
    thread->deleteLater();
}

void HPicSync::showOption(){
    if(this->mOptionWidget== NULL){
        this->mOptionWidget = new HPSOptionWidget(&this->mOption,this);
        connect(mOptionWidget,SIGNAL(comboBoxViewSelectedChanged(int)),this,SLOT(comboBoxViewChanged(int)));
        connect( mOptionWidget,SIGNAL(dirsRemoved(QStringList)), &mDirManager,SLOT(removeDirs(QStringList)));

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


}
void HPicSync::loadImages(){
    this->mPosImages =0;
    this->mTimer.start();
    int number =QThread::idealThreadCount();
    QDir dir(this->mOption.getQuellOrdner());
    QStringList fileNames = dir.entryList(QStringList() << "*.png");
    int partSize= fileNames.size()/number;
    int pos=0, end=-1;
    qDebug() << partSize<<fileNames.size();
    for (int  i = 0; i< number;i++){
        pos =end+1;
        if(i==number-1)
            end=fileNames.size()-1;
        else
            end+=partSize;
        qDebug() << pos << end;
        HPSImageLoader *imageLoader = new HPSImageLoader(mMutex,fileNames,pos,end,this->mOption.getQuellOrdner(),
                                                         this->mOption.getThumbSize(),
                                                         &this->mThumbs);
        QThread *thread = new QThread();
        this->mThreads.insert(thread,imageLoader);
        imageLoader->moveToThread(thread);
        this->connect(thread,SIGNAL(started()),imageLoader,SLOT(start()));
        this->connect(imageLoader,SIGNAL(destroyed()),thread,SLOT(quit()));
        this->connect(thread,SIGNAL(finished()),this,SLOT(threadClear()));
        //this->connect(imageLoader,SIGNAL(error(int,int,QString)),this,SLOT(fotosCheck(int,int,QString)));
        this->connect(imageLoader,SIGNAL(ready(int,QString)),this,SLOT(fotosReady(int,QString)));
        this->connect(imageLoader,SIGNAL(fertig()),this,SLOT(fertigTime()));
        thread->start();


    }
    this->mPixOldLoadCountLabel->setVisible(true);
    this->mPixOldLoadCountLabel->setText("0 images geladen");
}
void HPicSync::fotosReady(int size,const QString &str){
    qDebug() << this->mThumbs.size() <<this->mPosImages+size;
    this->mPosImages+=size;
    this->mPixOldLoadCountLabel->setText(QString::number(this->mPosImages)+" images geladen");
    if(size !=0 ){
        for(int i = this->mPosImages-4;i<mPosImages;i++){
            QListWidgetItem *item= new QListWidgetItem();

            QImage *img =&this->mThumbs[i];
            item->setData(Qt::DisplayRole,img->text("name"));
            item->setData(Qt::DecorationRole,qVariantFromValue((void *) img));
            //img =&this->fotos[i];
            item->setData(Qt::UserRole+2,qVariantFromValue((void *) img));
            item->setData(Qt::UserRole+1,i);
            item->setData(Qt::UserRole+3,this->mOption.getThumbSize());
            item->setCheckState(Qt::Unchecked);
            this->mOldListWidget->addItem(item);
        }
    }
    if(str !=""){
        QMessageBox::warning(this,"Fehler","Bei abrufen des Bildes: "+str+" ist ein Fehler aufgetreten",QMessageBox::Ok);
    }
}



void HPicSync::socketError(QAbstractSocket::SocketError error){
    qDebug() << "error "<< error;
}
void HPicSync::test2() {
    // qDebug() << this->socket->state();


}

void HPicSync::fertigTime(){
    qDebug() << "fertig" << this->mTimer.elapsed();
}

void HPicSync::comboBoxViewChanged(int index)
{
    initCBOrdner(index,mTreeComboBox->getCurrentDir());

}

void HPicSync::clickedPlus()
{
    const QString str = QDir::fromNativeSeparators(QFileDialog::getExistingDirectory(this,QDir::homePath()));
    qDebug() << str;
    if (!str.isEmpty()&&!mDirManager.dirs().contains(str)) {
        mDirManager.addDir(str);
        mTreeComboBox->updateText();
    }

}

void HPicSync::initCBOrdner(int index,const QString &dir)
{
    qDebug() << "initCBOrdner" << index << dir;
    if(index == HPSOption::ListView){
        mOption.setExpandDirs(mTreeComboBox->expandeDirs());
        mTreeComboBox->setViewToList();
    } else {
        mTreeComboBox->setViewToTree();
    }
   QList<QStandardItem*> expandesIems= mDirManager.makeView();
   if(!expandesIems.isEmpty())
    mTreeComboBox->setExpandedItems( expandesIems );
       mTreeComboBox->findeAndSetCurrentItem(dir);
}
