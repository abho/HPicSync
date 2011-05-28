#include "hpsoptionwidget.h"

HPSOptionWidget::HPSOptionWidget(HPSOption *option,QWidget *parent) :
    QWidget(parent),optionObject(option),options(4,false)
{
    this->setWindowModality(Qt::WindowModal);
    this->setWindowFlags(Qt::Window);
    QVBoxLayout *mainBox = new QVBoxLayout();

    QTabWidget *tab  = new QTabWidget(this);

    //ALLGEMEINWIDGET
    QGridLayout *allgemeinBox = new QGridLayout();
    QLabel *lQuellOrdner = new QLabel(tr("Standard Bildordner"));
    this->eQuellOrdner = new QLineEdit();
    this->eQuellOrdner->setText(this->optionObject->getQuellOrdner());
    this->butQuellOrdner = new QPushButton(tr("Durchsuchen"));
    allgemeinBox->addWidget(lQuellOrdner,0,0);
    allgemeinBox->addWidget(this->eQuellOrdner,0,1);
    allgemeinBox->addWidget(this->butQuellOrdner,0,2);

    QLabel *lSlider = new QLabel(trUtf8("Größe der Vorschaubilder:"));
    this->slider = new QSlider(Qt::Horizontal);
    this->slider->setMinimum(3);
    this->slider->setMaximum(15);
    this->slider->setSingleStep(10);
    this->slider->setTickInterval(1);
    this->slider->setValue(this->optionObject->getThumbSize()/20);
    this->slider->setTickPosition(QSlider::TicksBelow);
    this->lSliderWert = new QLabel(QString::number(this->slider->value()*20)+tr(" pix"));
    allgemeinBox->addWidget(lSlider,1,0);
    allgemeinBox->addWidget(this->slider,1,1);
    allgemeinBox->addWidget(this->lSliderWert,1,2);

    this->oldSliderValue =this->slider->value();
    QWidget *allgemeinWidget = new QWidget(this);
    allgemeinWidget->setLayout(allgemeinBox);
    tab->addTab(allgemeinWidget,trUtf8("Allgemein"));

    //ORDNERWIDGET
    QVBoxLayout * ordnerBox = new QVBoxLayout();
    listview = new QListWidget();
    listview->addItems( optionObject->getOrdner());
    ordnerBox->addWidget(listview);

    QGridLayout *comboViewBox = new QGridLayout();
    QPushButton *butRemove  =new QPushButton(trUtf8("Entfernen"));
    QLabel *lComboBoxView  = new QLabel(trUtf8("Ornderliste anzeigen als:"));
    this->comboBox = new QComboBox();
    this->comboBox->addItems(QStringList()<<trUtf8("Listenansicht") << trUtf8("Baumansicht"));
    if(this->optionObject->getComboBoxView() == this->optionObject->ListView)
        this->comboBox->setCurrentIndex(0);
    else
        this->comboBox->setCurrentIndex(1);    
    comboViewBox->addWidget(butRemove,0,2);
    comboViewBox->addWidget(lComboBoxView,1,0);
    comboViewBox->addWidget(this->comboBox,1,1,1,2);
    ordnerBox->addLayout(comboViewBox);
    QWidget * ordnerWidget = new QWidget(this);
    ordnerWidget->setLayout(ordnerBox);

    tab->addTab(ordnerWidget,trUtf8("Ordner"));


    QHBoxLayout *butBox = new QHBoxLayout();
    this->butUebernehmen = new QPushButton(trUtf8("Übernehmen"));
    this->butBeenden = new QPushButton(tr("Beenden"));
    butBox->addStretch();
    butBox->addWidget(this->butUebernehmen);
    butBox->addWidget(this->butBeenden);

    mainBox->addWidget(tab);
    mainBox->addLayout(butBox);
    this->setLayout(mainBox);


    this->connect(this->slider,SIGNAL(valueChanged(int)),this,SLOT(sliderChanged(int)));
    this->connect(this->butQuellOrdner,SIGNAL(clicked()),this,SLOT(quelleSuchen()));
    this->connect(this->butBeenden,SIGNAL(clicked()),this,SLOT(close()));
    this->connect(this->eQuellOrdner,SIGNAL(textChanged(QString)),this,SLOT(textchanged(QString)));
    this->connect(this->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxViewChanged(int)));
    connect(butUebernehmen,SIGNAL(clicked()),this,SLOT(uebernehmen()));
    connect(butRemove,SIGNAL(clicked()),this,SLOT(cRemoveDir()));


}
void HPSOptionWidget::resetAndShow(){
    this->options.fill(false);
    if(!this->optionObject->getGeometryOption().isNull()){
        this->setGeometry(this->optionObject->getGeometryOption());      
    }
    if (listview->count() != optionObject->getOrdner().size()) {
        loadOrdner();
    }
    this->setVisible(true);
}

void HPSOptionWidget::quelleSuchen() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Ordnerauswahl"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if(!dir.isEmpty()){
        this->eQuellOrdner->setText(dir);
    }
}
void HPSOptionWidget::uebernehmen() {
    for(int i = 0;i<this->options.size();i++){
        if(options.at(i)==true){
            switch(i) {
            case QuellOrdner:{
                this->optionObject->setQuellOrdner(this->eQuellOrdner->text());
                break;
            }
            case PixSize:{
                this->optionObject->setThumbSize(this->slider->value()*20);
                break;
            }
            case ComboBoxView:{
                qDebug() << "emit comboBox";
                this->optionObject->setComboBoxView(this->comboBox->currentIndex());
                emit comboBoxViewSelectedChanged(optionObject->getComboBoxView());
                break;
            }
            case DirList: {
                qDebug() << "DirList";
                //emit dirRemoved(checkRemovedDirs());
                dirItemList.clear();
            }
            }
        }
    }
    this->options.fill(false);
}
void HPSOptionWidget::textchanged(const QString & text) {
    qDebug() << text << this->optionObject->getQuellOrdner();
    if(text!=this->optionObject->getQuellOrdner()){
        this->options[QuellOrdner] = true;
    } else {
        this->options[QuellOrdner] =false;
    }
}

void HPSOptionWidget::closeEvent ( QCloseEvent * event ){
    qDebug() << this->options;
    if(this->options.contains(true)){
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
            this->uebernehmen();
            if(this->optionObject->getThumbSize()!= this->oldSliderValue){
                emit this->scale();
                this->oldSliderValue=this->optionObject->getThumbSize();
            }
            this->optionObject->setGeometryOption(this->geometry());
            this->hide();
            event->ignore();
        }else if(box.clickedButton() == nichtSpeichernButton){
            qDebug() << "nicht speichern";
            zuruecksetzten();
            this->hide();
            event->ignore();
        }else if(box.clickedButton() == cancelButton){
            qDebug() << "cancel";
            event->ignore();
        }
    } else {
        if(this->optionObject->getThumbSize() != this->oldSliderValue){
            emit this->scale();
            this->oldSliderValue=this->optionObject->getThumbSize();
        }
        event->ignore();
        this->optionObject->setGeometryOption(this->geometry());
        this->hide();
    }
}
void HPSOptionWidget::sliderChanged(int i) {
    qDebug()<< "slider" << i*20 << this->optionObject->getThumbSize();
    emit this->sliderTicked(i*20);
    if(this->slider->value()*20 !=this->optionObject->getThumbSize()){
        this->options[PixSize] = true;
    } else{
        this->options[PixSize] = false;
    }
    this->lSliderWert->setText(QString::number(this->slider->value()*20)+" pix");
}
void HPSOptionWidget::comboBoxViewChanged(int index){
    if(index != this->optionObject->getComboBoxView())
        this->options[ComboBoxView] = true;
    else
        this->options[ComboBoxView] = false;
}

void HPSOptionWidget::zuruecksetzten()
{
    for(int i = 0;i<this->options.size();i++){
        if(options.at(i)==true){
            switch(i) {
            case QuellOrdner:{
                eQuellOrdner->setText(optionObject->getQuellOrdner());
                break;
            }
            case PixSize:{
                slider->setValue(optionObject->getThumbSize()*20);
                break;
            }
            case ComboBoxView:{
                comboBox->setCurrentIndex(optionObject->getComboBoxView());
                break;
            }
            case DirList: {
                //dirChangedList.clear();
            }
            }
        }
    }
}

void HPSOptionWidget::loadOrdner()
{
    for (int var = listview->count(); var < optionObject->getOrdner().size(); ++var) {
        QListWidgetItem * item = new QListWidgetItem(optionObject->getOrdner().at(var),listview);

    }
}

void HPSOptionWidget::cRemoveDir()
{
    qDebug() << "cRemoveDir";
    //QStandardItem *item =  listview->takeItem(listview->currentItem());
    //dirChangedList.append(item);
    options[DirList] = true;
}

QStringList HPSOptionWidget::checkRemovedDirs()
{
    QStringList newList;
    QStandardItem *item;
    for (int var = 0; var < dirItemList.size(); ++var) {
        item = dirItemList.at(var);
        newList.append(item->text());
        delete item;
    }
    return newList;
}

void HPSOptionWidget::reinsertDirItems()
{
    QStandardItem *item;
    for (int var = 0; var < dirItemList.size(); ++var) {
        item = dirItemList.at(var);
        //listview->addItem(item);
    }
    dirItemList.clear();
}


