#include "hpspasswordwidget.h"

HPSPasswordWidget::HPSPasswordWidget(HPSOption *option, QWidget *parent) :
    QWidget(parent),mOption(option)
{
    setWindowFlags(Qt::Dialog);
    setWindowModality(Qt::ApplicationModal);
    QVBoxLayout *mainBox =new QVBoxLayout();
    mTextLabel = new QLabel();

    QLabel *lUsername = new QLabel(trUtf8("Username:"));
    mUsernameLineEdit = new QLineEdit(option->getUsername());
    QHBoxLayout *userBox = new QHBoxLayout();
    userBox->addWidget(lUsername);
    userBox->addWidget(mUsernameLineEdit);

    QLabel *lPassword = new QLabel(trUtf8("Password:"));
    mPasswordLineEdit = new QLineEdit(option->getPassword());
    mPasswordLineEdit->setEchoMode(QLineEdit::Password);
    QHBoxLayout *passBox = new QHBoxLayout();
    passBox->addWidget(lPassword);
    passBox->addWidget(mPasswordLineEdit);

    mButton1 = new QPushButton();
    mButton1->setVisible(false);
    mButton2 = new QPushButton();
    mButton2->setVisible(false);
    mButton3 = new QPushButton();
    mButton3->setVisible(false);
    mButtonList.append(mButton1);
    mButtonList.append(mButton2);
    mButtonList.append(mButton3);
    QHBoxLayout *buttonBox = new QHBoxLayout();
    buttonBox->addWidget(mButton1);
    buttonBox->addWidget(mButton2);
    buttonBox->addWidget(mButton3);

    mainBox->addWidget(mTextLabel);
    mainBox->addLayout(userBox);
    mainBox->addLayout(passBox);
    mainBox->addLayout(buttonBox);

    setLayout(mainBox);

    connect(mButton1,SIGNAL(clicked()),this,SLOT(clicked()));
    connect(mButton2,SIGNAL(clicked()),this,SLOT(clicked()));
    connect(mButton3,SIGNAL(clicked()),this,SLOT(clicked()));
}

void HPSPasswordWidget::clicked(){
 QPushButton * but = (QPushButton*)sender();
    for (int i = 0;i<mButtonList.size();i++){
        if(mButtonList.at(i)==but)
            emit finished(i);
    }
}
QStringList HPSPasswordWidget::getUserAndPassword(){
    QStringList list;
    list.append(mUsernameLineEdit->text());
    list.append(mPasswordLineEdit->text());
    return list;
}

void HPSPasswordWidget::setLabelText(const QString &str){
    mTextLabel->setText(str);
}

void HPSPasswordWidget::closeEvent(QCloseEvent *event){
    emit finished(-1);
    event->accept();
}

void HPSPasswordWidget::setButtonText(const QStringList &texte){
     for (int i =0;i<texte.size();i++){
        mButtonList.at(i)->setText(texte.at(i));
        mButtonList.at(i)->setVisible(true);
    }
}

