#ifndef HPSSELECTADDRESSWIDGET_H
#define HPSSELECTADDRESSWIDGET_H

#include <QtGui>
#include <QtNetwork>

class HPSSelectAddressWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HPSSelectAddressWidget(const QList<QTcpSocket*> &list,QWidget *parent = 0);
protected:
    void closeEvent(QCloseEvent *);
private:
    const QList<QTcpSocket*> &mSocketList;
signals:
    void selected(QTcpSocket*);
private slots:
    void clicked(int i);


};

#endif // HPSSELECTADDRESSWIDGET_H
