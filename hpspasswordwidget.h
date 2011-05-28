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
    HPSOption *option;
    QLabel *lText;
    QLineEdit *ePassword,*eUsername;
    QList<QPushButton *> buttonList;
    QPushButton *but1,*but2,*but3;
signals:
    void finished(int);

private slots:
    void clicked();

};

#endif // HPSPASSWORDWIDGET_H
