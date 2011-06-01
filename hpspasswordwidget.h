#ifndef HPSPASSWORDWIDGET_H
#define HPSPASSWORDWIDGET_H

#include <QtGui>
#include "hpsoption.h"

class HPSPasswordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HPSPasswordWidget(HPSOption *option,QWidget *parent = 0);
    void setLabelText(const QString &str);
    void setButtonText(const QStringList &texte);
    QStringList getUserAndPassword();
protected:
    void closeEvent(QCloseEvent *);

private:
    HPSOption *mOption;
    QLabel *mTextLabel;
    QLineEdit *mPasswordLineEdit,*mUsernameLineEdit;
    QList<QPushButton *> mButtonList;
    QPushButton *mButton1,*mButton2,*mButton3;
signals:
    void finished(int);

private slots:
    void clicked();

};

#endif // HPSPASSWORDWIDGET_H
