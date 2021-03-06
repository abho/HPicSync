#include "hpicsync.h"
#include <QDebug>

HPicSync::HPicSync(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::HPicSync),mOptionWidget(NULL),mThreadManager(this),mDirManager(mThreadManager,mDatabaseHandler,mOption),
      mThumbManager(mDatabaseHandler, mOption),mMoreThanOneSelected(false)

{
    ui->setupUi(this);

    ui->listWidgetNew->setItemDelegate(new HPSListViewDelegate(mOption.getThumbSize(),&mMoreThanOneSelected,this));
    ui->listWidgetOld->setItemDelegate(new HPSOldListDelegate(mOption.getThumbSize(),this));

    mDirManager.setModel( ui->comboBox->standardModel());
    initCBOrdner(mOption.getComboBoxView(),mOption.getComboBoxCurrentDir());


    QStatusBar *bar = ui->statusbar;
    mConnectLabel = new QLabel(tr("nicht verbunden"));
    mConnectPixGruenLabel = new QLabel();
    mConnectPixGruenLabel->setPixmap(QPixmap(":/knopfGruen").scaled(QSize(17,17),Qt::KeepAspectRatio));
    mConnectPixRotLabel = new QLabel();
    mConnectPixRotLabel->setPixmap(QPixmap(":/knopfRot").scaled(QSize(17,17),Qt::KeepAspectRatio));
    mBar = new HPSProgressBar;
    // this->mPixOldLoadCountLabel = new QLabel("");
    //this->mPixOldLoadCountLabel->setVisible(false);
    mBar->setVisible(false);
    mBar->setValue(0);
    mBar->setTextVisible(false);
    bar->addWidget(this->mConnectPixGruenLabel);
    bar->addWidget(this->mConnectPixRotLabel);
    bar->addWidget(this->mConnectLabel);
    bar->addPermanentWidget( mBar);
ui->progressBar->hide();
    /*
    this->connect(this->mCloseButton,SIGNAL(clicked()),this,SLOT(close()));
    this->connect(this->mOptionButton,SIGNAL(clicked()),this,SLOT(showOption()));
    this->connect(this->mRefreshButton,SIGNAL(clicked()),this,SLOT(test()));
    this->connect(this->mCopyButton,SIGNAL(clicked()),this,SLOT(test2()));

    connect(mPlusButton,SIGNAL(clicked()),this,SLOT(clickedPlus()));
    connect( &mThumbManager,SIGNAL(thumbsReady(int)),this,SLOT(refreshBar(int)));
    connect( &mThumbManager,SIGNAL(startThumbCreation(QString,int)),this,SLOT(initBar(QString,int)));
    connect( &mThumbManager,SIGNAL(dirCreationReady(QString)),&mDirManager,SLOT(finishAddDir(QString)));
    //connect( &mThumbManager,SIGNAL(startCreation()),this,SLOT(startBar()));
    connect( &mThumbManager,SIGNAL(creationReady()),this,SLOT(finishBar()));
    connect( mMinusButton,SIGNAL(clicked()),this,SLOT(clickedMinus()));

*/
    connect( ui->comboBox,SIGNAL(dirChanged(QString)),this,SLOT(comboBoxDirClicked(QString)));
    setGeometry(mOption.getGeometry());
    if(!mDatabaseHandler.openDatabase("picsync.db"))
        QMessageBox::critical(this, trUtf8("Fehler"), trUtf8("Verbindeung mit der Datenbank konnte nicht hergestellt werden."),QMessageBox::Ok);

    QDir dir( QApplication::applicationDirPath());
    if(!dir.exists(".thumbs")){
        dir.mkdir(".thumbs");
    }

    initThumbManager();
    qDebug() << Q_FUNC_INFO << mOption.dirFromDirlister();
    if(!mOption.dirFromDirlister().isEmpty()){
        mDirManager.startAddDir(mOption.dirFromDirlister(),true);
    }
    mThreadManager.initDirWatcher(mOption,mDirManager,mDatabaseHandler);
   HPSDirWatcher *dirWatcher =  mThreadManager.dirWatcher();
   connect(this,SIGNAL(startFirstRun()),dirWatcher,SLOT(startFirstRun()));
   emit startFirstRun();

}
HPicSync::~HPicSync()
{
    delete ui;
    /*

    //mOption.setExpandDirs( mTreeComboBox->expandeDirs());
    */
}
void HPicSync::closeEvent(QCloseEvent *event){
    qDebug() << "closeEvent" << mThreadManager.threadClosed();
    if(mThreadManager.threadClosed()){
        mOption.setGeometry(this->geometry());
        mOption.setComboBoxCurrentDir(ui->comboBox->currentDir());
        mDirManager.saveDirModel();
        mDirManager.reset();
        event->accept();
    } else {
        mThreadManager.closeAllThreads();
        connect(&mThreadManager,SIGNAL(allThreadsAreClosed()),this,SLOT(close()));
        event->ignore();
    }
}

void HPicSync::test(){

    //mDirManager.reset();



}

void HPicSync::socketError(QAbstractSocket::SocketError error){
    //qDebug() << "error "<< error;
}
void HPicSync::test2() {
    // qDebug() << this->socket->state();
}
void HPicSync::comboBoxViewChanged(int index)
{
    initCBOrdner(index,ui->comboBox->currentDir());
}



void HPicSync::comboBoxDirClicked(QString dir)
{
    qDebug() << Q_FUNC_INFO << dir;

    mThumbManager.loadThumbs(dir);
    mThreadManager.dirWatcher()->setWatcherOn(dir);

}








void HPicSync::initCBOrdner(int index,const QString &dir)
{
    qDebug() << "initCBOrdner" << index << dir;

    mDirManager.makeView();
    if(index == HPSOption::ListView){
        ui->comboBox->setView(HPSComboBox::ListView);
    } else {
        ui->comboBox->setView(HPSComboBox::TreeView);
        ui->comboBox->loadExpanded();
    }
    ui->comboBox->setItemByText(dir);
}

void HPicSync::initThumbManager()
{


    mThumbManager.setListWidget( ui->listWidgetOld);
    mThumbManager.loadThumbs( ui->comboBox->currentDir());
    //bool workToDo = mThumbManager.startWork();
    qDebug() << Q_FUNC_INFO ;

}





void HPicSync::on_butOption_clicked()
{
    if(mOptionWidget== NULL){
        mOptionWidget = new HPSOptionWidget(&mOption,this);
        connect(mOptionWidget,SIGNAL(comboBoxViewSelectedChanged(int)),this,SLOT(comboBoxViewChanged(int)));
        connect(mOptionWidget,SIGNAL(sliderTicked(int)),this,SLOT(onMoptionwidgetThumbsizechanged(int)));
    }
    mOptionWidget->resetAndShow();
}

QVector<QListWidgetItem *> & HPicSync::tmpListWidgetItems()
{
    return mTmpListWidgetItems;
}

void HPicSync::onImageLoaderThumbsReady(int pos, int count)
{
    for (int i = pos; i < pos+count; ++i) {
        ui->listWidgetOld->addItem(mTmpListWidgetItems[i]);
    }
}

void HPicSync::on_butPlus_clicked()
{
    const QString str = QDir::fromNativeSeparators(QFileDialog::getExistingDirectory(this,QDir::homePath()));
    //(qDebug() << str;
    if(!str.isEmpty()){
        QMessageBox dialog(this);
        dialog.setText( trUtf8("Unterordner auch hinzufügen?"));
        QPushButton *jaButton = dialog.addButton( trUtf8("Ja"),QMessageBox::YesRole);
        QPushButton *neinButton =  dialog.addButton(trUtf8("Nein"),QMessageBox::NoRole);
        QPushButton *abbruchButton = dialog.addButton(trUtf8("Abbruch"),QMessageBox::RejectRole);
        dialog.exec();

        if(dialog.clickedButton() == abbruchButton){
            qDebug() << "abbruch";
            return ;
        } else if(dialog.clickedButton() == jaButton) {
            qDebug() << "ja";
            //startBar();
           mDirManager.startAddDir(str,true);
        } else if(dialog.clickedButton() == neinButton) {
            qDebug() << "nein";
            //startBar();
            mDirManager.startAddDir(str,false);
        }
    }
}

void HPicSync::onMoptionwidgetThumbsizechanged(int size)
{
    qDebug() << size;
    ui->listWidgetOld->setItemDelegate(new HPSOldListDelegate(size,this));
    ui->listWidgetOld->setIconSize(QSize(size,size));

}

void HPicSync::onDirCheckerStartCheck(QString dir)
{
    if( !ui->progressBar->isVisible())
        ui->progressBar->show();
ui->progressBar->setText("creating thumbnails: "+dir+" ...");
}

void HPicSync::onDirCheckerFinished()
{
    ui->progressBar->hide();
}

void HPicSync::on_butMinus_clicked()
{

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
        mDirManager.removeDir( QDir::fromNativeSeparators( ui->comboBox->currentText()),true);
    } else if(dialog.clickedButton() == neinButton) {
        qDebug() << "nein";
        mDirManager.removeDir( QDir::fromNativeSeparators( ui->comboBox->currentText()),false);
    }
    ui->comboBox->clearSelection();
    mThreadManager.dirWatcher()->stopWatching();

}
