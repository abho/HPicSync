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

class HPSTreeCombobox : public QComboBox
{
    Q_OBJECT
public:
    HPSTreeCombobox(QModelIndex defaultRootIdx = QModelIndex(), QWidget* parent = 0);
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
    void updateText();
    void blockLineEditChanged(const QString &);
    void itemClicked( QModelIndex index);

private:
    bool skipNextHide;
    QTreeView *treeView;
    QListView *listView;
protected:
    QModelIndex defaultRootIndex;
    QModelIndex cIndex;
};




class StandardHPSTreeCombobox : public HPSTreeCombobox
{
    Q_OBJECT

public:
    StandardHPSTreeCombobox(QWidget* parent = 0);

    QStandardItemModel *model() const;
    // convenience method to add item at the top of the tree
    void findeAndSetCurrentItem(const QString &dir);
    void setCurrentItem(QStandardItem *item);
    const QString getCurrentDir();
    QStringList expandeDirs();
private slots:
    void saveExpandItem(QModelIndex &);
private:
    QStandardItem* findIndex( QStandardItem *item,const QString &dir);

    QStringList expandeDirs_;
};
