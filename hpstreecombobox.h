// Code to create a QComboBox where multiple items can be selected and the items
// are shown in a tree.
// Written by Corwin Joy
//  code is hereby placed in the Public Domain.
//  code comes with no warranty of any kind, use at your own risk.

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
        mDefaultRootIndex = defaultRootIdx;
    }
    void setViewToTree();
    void setViewToList();
    bool eventFilter(QObject* object, QEvent* event);
    virtual void showPopup();
    virtual void hidePopup();
    void setExpandedItems(const QList<QStandardItem*> &items);


public Q_SLOTS:
    void updateText();
    void blockLineEditChanged(const QString &);
    void itemClicked( QModelIndex index);

private slots:
    void saveExpandItem(const QModelIndex &index);
    void removeExpandItem(const  QModelIndex &index);
private:
    bool mSkipNextHide;
    QTreeView *mTreeView;
    QListView *mListView;
protected:
    QModelIndex mDefaultRootIndex;
    QModelIndex mCIndex;
    QStringList mExpandeDirs;
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
    const QStringList &expandeDirs();

private:
    QStandardItem* findIndex( QStandardItem *item,const QString &dir);


};
