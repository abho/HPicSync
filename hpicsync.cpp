#include "hpicsync.h"
#include <QDebug>

HPicSync::HPicSync(QWidget *parent)
    : QMainWindow(parent),optionWidget(NULL),dirManager(option),moreThanOneSelected(false)
{


    QHBoxLayout *optionBox = new QHBoxLayout();
    this->butOption = new QPushButton(tr("Option"));
    optionBox->addStretch();
    optionBox->addWidget(this->butOption);

    QHBoxLayout *viewBox= new QHBoxLayout();

    QVBoxLayout *newListBox= new QVBoxLayout();
    QHBoxLayout *refreshBox = new QHBoxLayout();
    this->butRefresh = new QPushButton(tr("Refresh"));
    refreshBox->addWidget(butRefresh);
    refreshBox->addStretch();

    this->newList = new QListWidget();
    this->newList->setFlow(QListView::LeftToRight);
    this->newList->setWrapping(true);
    this->newList->setUniformItemSizes(true);
    this->newList->setResizeMode(QListView::Adjust);
    this->newList->setAutoScroll(false);
    this->newList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    HPSListViewDelegate *delegateNew = new HPSListViewDelegate(this->option.getThumbSize(),&this->moreThanOneSelected,this);
    this->newList->setItemDelegate(delegateNew);


    QHBoxLayout * buttonBox = new QHBoxLayout();
    this->butMarkAll = new QPushButton(trUtf8("Alle auswählen"));
    this->butMarkSelected = new QPushButton(trUtf8("Markierte auswählen"));
    buttonBox->addWidget(this->butMarkAll);
    buttonBox->addWidget(this->butMarkSelected);
    buttonBox->addStretch();

    newListBox->addLayout(refreshBox);
    newListBox->addWidget(newList);
    newListBox->addLayout(buttonBox);

    this->butCopy = new QPushButton(tr("copy"));

    QVBoxLayout *oldListBox = new QVBoxLayout();


    QHBoxLayout *comboBox = new QHBoxLayout();

    this->coOrdner = new Standardhpstreecombobox(this);
    initCBOrdner(option.getComboBoxView(),option.getComboBoxCurrentDir());
    butPlus=new QPushButton(tr("plus"));
    comboBox->addWidget(coOrdner,10);
    comboBox->addWidget(butPlus,1);

    this->oldList = new QListWidget();
    this->oldList->setFlow(QListView::LeftToRight);
    this->oldList->setWrapping(true);
    this->oldList->setUniformItemSizes(true);
    this->oldList->setResizeMode(QListView::Adjust);
    this->oldList->setAutoScroll(false);
    this->oldList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    HPSListViewDelegate *delegateOld = new HPSListViewDelegate(this->option.getThumbSize(),&this->moreThanOneSelected,this);
    this->oldList->setItemDelegate(delegateOld);

    QHBoxLayout *beendenBox = new QHBoxLayout();
    this->butClose = new QPushButton(tr("Beenden"));
    beendenBox->addStretch();
    beendenBox->addWidget(this->butClose);

    oldListBox->addLayout(comboBox);
    oldListBox->addWidget(oldList);
    qDebug() << butPlus->font().pointSize();
    oldListBox->addLayout(beendenBox);
    viewBox->addLayout(newListBox);
    viewBox->addWidget(this->butCopy);
    viewBox->addLayout(oldListBox);

    QVBoxLayout *mainBox = new QVBoxLayout();
    mainBox->addLayout(optionBox);
    mainBox->addLayout(viewBox);

    QWidget *mainWidget = new QWidget();
    mainWidget->setLayout(mainBox);
    this->setCentralWidget(mainWidget);

    QStatusBar *bar = new QStatusBar();
    this->setStatusBar(bar);
    this->lConnect = new QLabel(tr("nicht verbunden"));
    this->lConnectPixGruen = new QLabel();
    this->lConnectPixGruen->setPixmap(QPixmap(":/knopfGruen").scaled(QSize(17,17),Qt::KeepAspectRatio));
    this->lConnectPixRot = new QLabel();
    this->lConnectPixRot->setPixmap(QPixmap(":/knopfRot").scaled(QSize(17,17),Qt::KeepAspectRatio));
    this->lPixOldLoadCount = new QLabel("");
    this->lPixOldLoadCount->setVisible(false);

    this->statusBar()->addWidget(this->lConnectPixGruen);
    this->statusBar()->addWidget(this->lConnectPixRot);
    this->statusBar()->addWidget(this->lConnect);
    this->statusBar()->addPermanentWidget(this->lPixOldLoadCount);

    this->connect(this->butClose,SIGNAL(clicked()),this,SLOT(close()));
    this->connect(this->butOption,SIGNAL(clicked()),this,SLOT(showOption()));
    this->connect(this->butRefresh,SIGNAL(clicked()),this,SLOT(test()));
    this->connect(this->butCopy,SIGNAL(clicked()),this,SLOT(test2()));
    connect(butPlus,SIGNAL(clicked()),this,SLOT(clickedPlus()));

    this->setGeometry(this->option.getGeometry());

}
HPicSync::~HPicSync()
{
    this->option.setGeometry(this->geometry());
    this->option.setComboBoxCurrentDir(coOrdner->getCurrentDir());

}





void HPicSync::closeEvent(QCloseEvent *event){
    qDebug() << "closeEvent" << this->threads;
    if(this->threads.isEmpty()){
        event->accept();
    } else {
        QMapIterator<QThread *, HPSImageLoader*> i(this->threads);
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
    this->threads.remove(thread);
    thread->deleteLater();
}

void HPicSync::showOption(){
    if(this->optionWidget== NULL){
        this->optionWidget = new HPSOptionWidget(&this->option,this);
        connect(optionWidget,SIGNAL(comboBoxViewSelectedChanged(int)),this,SLOT(comboBoxViewChanged(int)));
        connect( optionWidget,SIGNAL(dirsRemoved(QStringList)), &dirManager,SLOT(removeDirs(QStringList)));

        qDebug() << "optionWidget is null";
    }
    this->optionWidget->resetAndShow();
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
    this->posImages =0;
    this->timer.start();
    int number =QThread::idealThreadCount();
    QDir dir(this->option.getQuellOrdner());
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
        HPSImageLoader *imageLoader = new HPSImageLoader(mutex,fileNames,pos,end,this->option.getQuellOrdner(),
                                                         this->option.getThumbSize(),
                                                         &this->thumbs);
        QThread *thread = new QThread();
        this->threads.insert(thread,imageLoader);
        imageLoader->moveToThread(thread);
        this->connect(thread,SIGNAL(started()),imageLoader,SLOT(start()));
        this->connect(imageLoader,SIGNAL(destroyed()),thread,SLOT(quit()));
        this->connect(thread,SIGNAL(finished()),this,SLOT(threadClear()));
        //this->connect(imageLoader,SIGNAL(error(int,int,QString)),this,SLOT(fotosCheck(int,int,QString)));
        this->connect(imageLoader,SIGNAL(ready(int,QString)),this,SLOT(fotosReady(int,QString)));
        this->connect(imageLoader,SIGNAL(fertig()),this,SLOT(fertigTime()));
        thread->start();


    }
    this->lPixOldLoadCount->setVisible(true);
    this->lPixOldLoadCount->setText("0 images geladen");
}
void HPicSync::fotosReady(int size,const QString &str){
    qDebug() << this->thumbs.size() <<this->posImages+size;
    this->posImages+=size;
    this->lPixOldLoadCount->setText(QString::number(this->posImages)+" images geladen");
    if(size !=0 ){
        for(int i = this->posImages-4;i<posImages;i++){
            QListWidgetItem *item= new QListWidgetItem();

            QImage *img =&this->thumbs[i];
            item->setData(Qt::DisplayRole,img->text("name"));
            item->setData(Qt::DecorationRole,qVariantFromValue((void *) img));
            //img =&this->fotos[i];
            item->setData(Qt::UserRole+2,qVariantFromValue((void *) img));
            item->setData(Qt::UserRole+1,i);
            item->setData(Qt::UserRole+3,this->option.getThumbSize());
            item->setCheckState(Qt::Unchecked);
            this->oldList->addItem(item);
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
    qDebug() << "fertig" << this->timer.elapsed();
}

void HPicSync::comboBoxViewChanged(int index)
{
    initCBOrdner(index,coOrdner->getCurrentDir());

}

void HPicSync::clickedPlus()
{
    const QString str =QFileDialog::getExistingDirectory(this,QDir::homePath());
    if (!str.isEmpty()&&!dirManager.dirs().contains(str)) {
        dirManager.addDir(str);
    }
}

void HPicSync::initCBOrdner(int index,const QString &dir)
{
    qDebug() << "initCBOrdner" << index << dir;
    switch(index){
    case HPSOption::ListView:{
        coOrdner->setViewToList();
        dirManager.makeListView(dir,coOrdner->model());
        break;
    }
    case HPSOption::TreeView: {
        coOrdner->setViewToTree();
        dirManager.makeTreeView(dir,coOrdner->model());
        break;
    }
    }
    qDebug()  << "currentItem" << dirManager.getCurrentItem();
    coOrdner->setCurrentItem(dirManager.getCurrentItem());
}
