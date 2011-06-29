#ifndef HPSTREECOMBOBOX_H
#define HPSTREECOMBOBOX_H

#include <QComboBox>
#include <QDebug>
#include <QtCore>
#include <QtGui>
#include "hpspopupwidget.h"

class HPSComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit HPSComboBox(QWidget *parent = 0);
    void showPopup();
    enum View {
        ListView,TreeView
    };
    void setView( int view);
    void setItemByText(const QString &str);
    void loadExpanded();
    QStandardItemModel * standardModel();
    const QString  currentDir();


signals:

private slots:
    void onMPopupItemClicked(QString);
    void onMPopupExit();
private:
    HPSPopupWidget *mPopup;
    QLineEdit *mLineEdit;

    void initPopup();
    QStandardItem *findItemInList(const QString &dir);
        QStandardItem *findItemInTree(const QString &dir);

    void loadExpandedItem(QStandardItem * item);
};

#endif // HPSTREECOMBOBOX_H
