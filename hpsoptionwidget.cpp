#include "hpsoptionwidget.h"

HPSOptionWidget::HPSOptionWidget(HPSOption *option,QWidget *parent) :
    QWidget(parent),mOptionObject(option),mOptions(4,false)
{
    setWindowModality(Qt::WindowModal);
    setWindowFlags(Qt::Window);
    QVBoxLayout *mainBox = new QVBoxLayout();

    QTabWidget *tab  = new QTabWidget();

    //ALLGEMEINWIDGET
    QGridLayout *allgemeinBox = new QGridLayout();
    QLabel *lQuellOrdner = new QLabel(tr("Standard Bildordner"));
    mQuellOrdnerEditLine = new QLineEdit();
    mQuellOrdnerEditLine->setText(mOptionObject->getQuellOrdner());
    mQuellOrdnerButton = new QPushButton(tr("Durchsuchen"));
    allgemeinBox->addWidget(lQuellOrdner,0,0);
    allgemeinBox->addWidget(mQuellOrdnerEditLine,0,1);
    allgemeinBox->addWidget(mQuellOrdnerButton,0,2);

    QLabel *lSlider = new QLabel(trUtf8("Größe der Vorschaubilder:"));
    mSlider = new QSlider(Qt::Horizontal);
    mSlider->setMinimum(3);
    mSlider->setMaximum(15);
    mSlider->setSingleStep(10);
    mSlider->setTickInterval(1);
    mSlider->setValue(mOptionObject->getThumbSize()/20);
    mSlider->setTickPosition(QSlider::TicksBelow);
    mSliderWertLabel = new QLabel(QString::number(mSlider->value()*20)+tr(" pix"));
    allgemeinBox->addWidget(lSlider,1,0);
    allgemeinBox->addWidget(mSlider,1,1);
    allgemeinBox->addWidget(mSliderWertLabel,1,2);

    mOldSliderValue =mSlider->value();
    QWidget *allgemeinWidget = new QWidget();
    allgemeinWidget->setLayout(allgemeinBox);
    tab->addTab(allgemeinWidget,trUtf8("Allgemein"));

    //ORDNERWIDGET
    QVBoxLayout * ordnerBox = new QVBoxLayout();
    mListWidget = new QListWidget();
    loadOrdner();
    ordnerBox->addWidget(mListWidget);

    QGridLayout *comboViewBox = new QGridLayout();
    QPushButton *butRemove  =new QPushButton(trUtf8("Entfernen"));
    QLabel *lComboBoxView  = new QLabel(trUtf8("Ornderliste anzeigen als:"));
    mComboBox = new QComboBox();
    mComboBox->addItems(QStringList()<<trUtf8("Listenansicht") << trUtf8("Baumansicht"));
    if(mOptionObject->getComboBoxView() == mOptionObject->ListView)
        mComboBox->setCurrentIndex(0);
    else
        mComboBox->setCurrentIndex(1);
    comboViewBox->addWidget(butRemove,0,2);
    comboViewBox->addWidget(lComboBoxView,1,0);
    comboViewBox->addWidget(mComboBox,1,1,1,2);
    ordnerBox->addLayout(comboViewBox);
    QWidget * ordnerWidget = new QWidget();
    ordnerWidget->setLayout(ordnerBox);

    tab->addTab(ordnerWidget,trUtf8("Ordner"));

//testWidget............
    QHBoxLayout *testBox = new QHBoxLayout();
    QPushButton *testButton = new QPushButton("test");
    QWidget *w = new QWidget();
    testBox->addWidget( testButton);
    w->setLayout(testBox);
    tab->addTab( w,"test");
    connect( testButton,SIGNAL(clicked()),this,SLOT(test()));
    //-----------------
    QHBoxLayout *butBox = new QHBoxLayout();
    mUebernehmenButton = new QPushButton(trUtf8("Übernehmen"));
    mBeendenButton = new QPushButton(tr("Beenden"));
    butBox->addStretch();
    butBox->addWidget(mUebernehmenButton);
    butBox->addWidget(mBeendenButton);

    mainBox->addWidget(tab);
    mainBox->addLayout(butBox);
    setLayout(mainBox);


    connect(mSlider,SIGNAL(valueChanged(int)),this,SLOT(sliderChanged(int)));
    connect(mQuellOrdnerButton,SIGNAL(clicked()),this,SLOT(quelleSuchen()));
    connect(mBeendenButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(mQuellOrdnerEditLine,SIGNAL(textChanged(QString)),this,SLOT(textchanged(QString)));
    connect(mComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxViewChanged(int)));
    connect(mUebernehmenButton,SIGNAL(clicked()),this,SLOT(uebernehmen()));
    connect(butRemove,SIGNAL(clicked()),this,SLOT(cRemoveDir()));


}
void HPSOptionWidget::resetAndShow(){
    mOptions.fill(false);
    if(!mOptionObject->getGeometryOption().isNull()){
        setGeometry(mOptionObject->getGeometryOption());
    }
    if (mListWidget->count() != mOptionObject->getOrdner().size()) {
        loadOrdner();
    }
    setVisible(true);
}

void HPSOptionWidget::quelleSuchen() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Ordnerauswahl"),
                                                    QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!dir.isEmpty()){
        mQuellOrdnerEditLine->setText(dir);
    }
}
void HPSOptionWidget::uebernehmen() {
    for(int i = 0;i<mOptions.size();i++){
        if(mOptions.at(i)==true){
            switch(i) {
            case QuellOrdner:{
                mOptionObject->setQuellOrdner(mQuellOrdnerEditLine->text());
                break;
            }
            case PixSize:{
                mOptionObject->setThumbSize(mSlider->value()*20);
                break;
            }
            case ComboBoxView:{
                qDebug() << "emit comboBox";
                mOptionObject->setComboBoxView(mComboBox->currentIndex());
                emit comboBoxViewSelectedChanged(mOptionObject->getComboBoxView());
                break;
            }
            case DirList: {
                qDebug() << "DirList";
                emit dirsRemoved(checkRemovedDirs());
                mDirItemList.clear();
            }
            }
        }
    }
    mOptions.fill(false);
}
void HPSOptionWidget::textchanged(const QString & text) {
    qDebug() << text << mOptionObject->getQuellOrdner();
    if(text!=mOptionObject->getQuellOrdner()){
        mOptions[QuellOrdner] = true;
    } else {
        mOptions[QuellOrdner] =false;
    }
}

void HPSOptionWidget::closeEvent ( QCloseEvent * event ){
    qDebug() << mOptions;
    if(mOptions.contains(true)){
        QMessageBox box(this);
        box.setText(QString::fromUtf8("Die Daten wurden verändert."));
        box.setInformativeText("Sollen die Daten gespeichert werden?");
        QPushButton *speichernButton = box.addButton("Speichern",QMessageBox::AcceptRole);
        QPushButton *nichtSpeichernButton =box.addButton("Nicht speichern",QMessageBox::DestructiveRole);
        QPushButton *cancelButton=box.addButton("Abbrechen",QMessageBox::RejectRole);
        box.setIcon(QMessageBox::Question);
        box.exec();
        qDebug() << box.clickedButton();
        if(box.clickedButton() == speichernButton){
            qDebug() << "speichern";
            uebernehmen();
            if(mOptionObject->getThumbSize()!= mOldSliderValue){
                emit scale();
                mOldSliderValue=mOptionObject->getThumbSize();
            }
            mOptionObject->setGeometryOption(geometry());
            hide();
            event->ignore();
        }else if(box.clickedButton() == nichtSpeichernButton){
            qDebug() << "nicht speichern";
            zuruecksetzten();
            hide();
            event->ignore();
        }else if(box.clickedButton() == cancelButton){
            qDebug() << "cancel";
            event->ignore();
        }
    } else {
        if(mOptionObject->getThumbSize() != mOldSliderValue){
            emit scale();
            mOldSliderValue=mOptionObject->getThumbSize();
        }
        event->ignore();
        mOptionObject->setGeometryOption(geometry());
        hide();
    }
}
void HPSOptionWidget::sliderChanged(int i) {
    qDebug()<< "slider" << i*20 << mOptionObject->getThumbSize();
    emit sliderTicked(i*20);
    if(mSlider->value()*20 !=mOptionObject->getThumbSize()){
        mOptions[PixSize] = true;
    } else{
        mOptions[PixSize] = false;
    }
    mSliderWertLabel->setText(QString::number(mSlider->value()*20)+" pix");
}
void HPSOptionWidget::comboBoxViewChanged(int index){
    if(index != mOptionObject->getComboBoxView())
        mOptions[ComboBoxView] = true;
    else
        mOptions[ComboBoxView] = false;
}

void HPSOptionWidget::zuruecksetzten()
{
    for(int i = 0;i<mOptions.size();i++){
        if(mOptions.at(i)==true){
            switch(i) {
            case QuellOrdner:{
                mQuellOrdnerEditLine->setText(mOptionObject->getQuellOrdner());
                break;
            }
            case PixSize:{
                mSlider->setValue(mOptionObject->getThumbSize()*20);
                break;
            }
            case ComboBoxView:{
                mComboBox->setCurrentIndex(mOptionObject->getComboBoxView());
                break;
            }
            case DirList: {
                reinsertDirItems();
                mDirItemList.clear();
                break;
            }
            }
        }
    }
}

void HPSOptionWidget::loadOrdner()
{
    for (int var = mListWidget->count(); var < mOptionObject->getOrdner().size(); ++var) {
        QListWidgetItem * item = new QListWidgetItem(QDir::toNativeSeparators(mOptionObject->getOrdner().at(var)),mListWidget);

    }
}

void HPSOptionWidget::cRemoveDir()
{
    qDebug() << "cRemoveDir" << mListWidget->currentRow();
    QListWidgetItem *item =  mListWidget->takeItem(mListWidget->currentRow());
    mDirItemList.append(item);
    mOptions[DirList] = true;
}

QStringList HPSOptionWidget::checkRemovedDirs()
{
    QStringList newList;
    QListWidgetItem *item;
    for (int var = 0; var < mDirItemList.size(); ++var) {
        item = mDirItemList.at(var);
        newList.append(QDir::fromNativeSeparators(item->text()));
        delete item;
    }
    return newList;
}

void HPSOptionWidget::reinsertDirItems()
{
    for (int var = 0; var < mDirItemList.size(); ++var) {
        mListWidget->addItem( mDirItemList.at(var));
    }

}

void HPSOptionWidget::test()
{
    mOptionObject->test();
}


