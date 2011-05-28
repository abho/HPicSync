#include "hpspasswordwidget.h"

HPSPasswordWidget::HPSPasswordWidget(HPSOption *option, QWidget *parent) :
    QWidget(parent),option(option)
{
    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::ApplicationModal);
    QVBoxLayout *mainBox =new QVBoxLayout();
    this->lText = new QLabel();

    QLabel *lUsername = new QLabel(trUtf8("Username:"));
    this->eUsername = new QLineEdit(option->getUsername());
    QHBoxLayout *userBox = new QHBoxLayout();
    userBox->addWidget(lUsername);
    userBox->addWidget(this->eUsername);

    QLabel *lPassword = new QLabel(trUtf8("Password:"));
    this->ePassword = new QLineEdit(option->getPassword());
    this->ePassword->setEchoMode(QLineEdit::Password);
    QHBoxLayout *passBox = new QHBoxLayout();
    passBox->addWidget(lPassword);
    passBox->addWidget(this->ePassword);

    this->but1 = new QPushButton();
    this->but1->setVisible(false);
    this->but2 = new QPushButton();
    this->but2->setVisible(false);
    this->but3 = new QPushButton();
    this->but3->setVisible(false);
    this->buttonList.append(this->but1);
    this->buttonList.append(this->but2);
    this->buttonList.append(this->but3);
    QHBoxLayout *buttonBox = new QHBoxLayout();
    buttonBox->addWidget(this->but1);
    buttonBox->addWidget(this->but2);
    buttonBox->addWidget(this->but3);

    mainBox->addWidget(this->lText);
    mainBox->addLayout(userBox);
    mainBox->addLayout(passBox);
    mainBox->addLayout(buttonBox);

    this->setLayout(mainBox);

    this->connect(this->but1,SIGNAL(clicked()),this,SLOT(clicked()));
    this->connect(this->but2,SIGNAL(clicked()),this,SLOT(clicked()));
    this->connect(this->but3,SIGNAL(clicked()),this,SLOT(clicked()));
}

void HPSPasswordWidget::clicked(){
 QPushButton * but = (QPushButton*)sender();
    for (int i = 0;i<this->buttonList.size();i++){
        if(this->buttonList.at(i)==but)
            emit finished(i);
    }
}
QStringList HPSPasswordWidget::getUserAndPassword(){
    QStringList list;
    list.append(this->eUsername->text());
    list.append(this->ePassword->text());
    return list;
}

void HPSPasswordWidget::setLabelText(const QString &str){
    this->lText->setText(str);
}

void HPSPasswordWidget::closeEvent(QCloseEvent *event){
    emit this->finished(-1);
    event->accept();
}

void HPSPasswordWidget::setButtonText(const QStringList &texte){
     for (int i =0;i<texte.size();i++){
        this->buttonList.at(i)->setText(texte.at(i));
        this->buttonList.at(i)->setVisible(true);
    }
}

