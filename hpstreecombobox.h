// Code to create a QComboBox where multiple items can be selected and the items
// are shown in a tree.
// Written by Corwin Joy
// This code is hereby placed in the Public Domain.
// This code comes with no warranty of any kind, use at your own risk.

#pragma once
#include <QtGui/QComboBox>
#include <QtCore/QList>
#include <QtGui/QTreeView>
#include <QStandardItem>
#include <QListView>
#include <QtCore/QStringList>


class QStandardItemModel;

class hpstreecombobox : public QComboBox
{
    Q_OBJECT
public:
    hpstreecombobox(QModelIndex defaultRootIdx = QModelIndex(), QWidget* parent = 0);
    void setRoot(QModelIndex defaultRootIdx)
    {
        defaultRootIndex = defaultRootIdx;
    }
    void setViewToTree();
    void setViewToList();
    bool eventFilter(QObject* object, QEvent* event);
    virtual void showPopup();
    virtual void hidePopup();


public Q_SLOTS:
    //void setEditText (const QString & text);
    void updateText();
    void blockLineEditChanged(const QString &);

private:
    bool skipNextHide;    
    QTreeView *treeView;
    QListView *listView;
protected:
    QModelIndex defaultRootIndex;
    QModelIndex cIndex;
};




class Standardhpstreecombobox : public hpstreecombobox
{
    Q_OBJECT

public:
    Standardhpstreecombobox(QWidget* parent = 0);

    QStandardItemModel *model() const;
    // convenience method to add item at the top of the tree
    void addItem(const QString &text, const QVariant &userData = QVariant());
    void findeAndSetCurrentItem(const QString &dir);
    void setCurrentItem(QStandardItem *item);
    const QString getCurrentDir();
private:
    QModelIndex findIndex(const QModelIndex &index,const QString &dir);


};
