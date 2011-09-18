#include "hpsoptionwidget.h"

HPSOptionWidget::HPSOptionWidget(HPSOption *option,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HPSOptionWidget),
    mOptionObject(option),mOptions(4,false)
{
    ui->setupUi(this);
    setWindowModality(Qt::WindowModal);
    setWindowFlags(Qt::Window);
    ui->sliderThumbSize->setValue(mOptionObject->getThumbSize()/20);
    if(mOptionObject->getComboBoxView() == mOptionObject->ListView)
        ui->comboBoxDirsView->setCurrentIndex(HPSOption::ListView);
    else
        ui->comboBoxDirsView->setCurrentIndex(HPSOption::TreeView);
    ui->labelSliderWert->setText(QString::number(ui->sliderThumbSize->value()*20)+" pix");


}

HPSOptionWidget::~HPSOptionWidget(){
    delete ui;
}

void HPSOptionWidget::resetAndShow(){
    mOptions.fill(false);
    if(!mOptionObject->getGeometryOption().isNull()){
        setGeometry(mOptionObject->getGeometryOption());
    }
    setVisible(true);
}


void HPSOptionWidget::closeEvent ( QCloseEvent * event ){
  on_butCancel_clicked();
  event->ignore();
}

void HPSOptionWidget::zuruecksetzten()
{
    for(int i = 0;i<mOptions.size();i++){
        if(mOptions.at(i)==true){
            switch(i) {
            case PixSize:{
                ui->sliderThumbSize->setValue(mOptionObject->getThumbSize()*20);
                break;
            }
            case ComboBoxView:{
                ui->comboBoxDirsView->setCurrentIndex(mOptionObject->getComboBoxView());
                break;
            }
            }
        }
    }
}

void HPSOptionWidget::on_sliderThumbSize_valueChanged(int value)
{
    qDebug()<< "slider" << value*20 << mOptionObject->getThumbSize();
    emit sliderTicked(value*20);
    if(value*20 !=mOptionObject->getThumbSize()){
        mOptions[PixSize] = true;
    } else{
        mOptions[PixSize] = false;
    }
    ui->labelSliderWert->setText(QString::number(value*20)+" pix");
}

void HPSOptionWidget::on_comboBoxDirsView_currentIndexChanged(int index)
{
    if(index != mOptionObject->getComboBoxView())
        mOptions[ComboBoxView] = true;
    else
        mOptions[ComboBoxView] = false;

}

void HPSOptionWidget::on_butOk_clicked()
{
    for(int i = 0;i<mOptions.size();i++){
        if(mOptions.at(i)==true){
            switch(i) {
            case PixSize:{
                mOptionObject->setThumbSize(ui->sliderThumbSize->value()*20);
                break;
            }
            case ComboBoxView:{
                mOptionObject->setComboBoxView(ui->comboBoxDirsView->currentIndex());
                emit comboBoxViewSelectedChanged(mOptionObject->getComboBoxView());
                break;
            }
            }
        }
    }
    mOptions.fill(false);
    mOptionObject->setGeometryOption(geometry());
    hide();
}

void HPSOptionWidget::on_butCancel_clicked()
{
    for(int i = 0;i<mOptions.size();i++){
        if(mOptions.at(i)==true){
            switch(i) {
            case PixSize:{
                ui->sliderThumbSize->setValue(mOptionObject->getThumbSize()/20);
                break;
            }
            case ComboBoxView:{
                ui->comboBoxDirsView->setCurrentIndex(mOptionObject->getComboBoxView());
                break;
            }
            }
        }
    }
    mOptionObject->setGeometryOption(geometry());
    hide();
}
