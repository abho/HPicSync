#include "hpsselectaddresswidget.h"

HPSSelectAddressWidget::HPSSelectAddressWidget(const QList<QTcpSocket *> &list, QWidget *parent):
        QWidget(parent),list(list)
{
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowFlags(Qt::Window);
    QVBoxLayout *mainBox = new QVBoxLayout();
    QLabel *l = new QLabel();
    l->setText(trUtf8("Bitte Device wählen."));
     QSignalMapper *mapper =  new QSignalMapper(this);
    for(int i = 0 ;i< list.size();i++){
        QPushButton *but = new QPushButton(list.at(i)->peerName());
        this->connect(but,SIGNAL(clicked()),mapper,SLOT(map()));
        mapper->setMapping(but,i);
        mainBox->addWidget(but);    }
    connect(mapper,SIGNAL(mapped(int)),this,SLOT(clicked(int)));
    QPushButton *closeBut = new QPushButton(trUtf8("Abbrechen"));
    mainBox->addStretch();
    mainBox->addWidget(closeBut);
    this->connect(closeBut,SIGNAL(clicked()),this,SLOT(close()));
    this->setLayout(mainBox);
}
void HPSSelectAddressWidget::closeEvent(QCloseEvent *event){
    emit this->selected(NULL);
    event->accept();
}

void HPSSelectAddressWidget::clicked(int i) {
    emit this->selected(this->list.at(i));
}
