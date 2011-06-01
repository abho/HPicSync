//////////////////////////////////////////////////////////////////////////////////////////
// Code to create a QComboBox where multiple items can be selected and the items
// are shown in a tree.
// Written by Corwin Joy
//  code is hereby placed in the Public Domain.
//  code comes with no warranty of any kind, use at your own risk.qwe c

#include "hpstreecombobox.h"
#include <QtGui/QLineEdit>
#include <QtGui/QAbstractItemView>
#include <QtGui/QStandardItemModel>
#include <QtCore/QModelIndex>
#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
#include <QtGui/QHeaderView>
#include<QtCore/QEvent>
#include<QtGui/QMouseEvent>
#include<QtCore/QModelIndex>
#include <QListView>
#include <QDebug>
#include <cassert>
#include <QDir>



HPSTreeCombobox::HPSTreeCombobox(QWidget* parent) :
    QComboBox(parent), mSkipNextHide(false), mTreeView(NULL),mListView(NULL)

{
    view()->setSelectionMode(QAbstractItemView::SingleSelection);

    setEditable(true);
    lineEdit()->setReadOnly(true);
    setInsertPolicy(QComboBox::NoInsert);
    QStandardItemModel *model  = new QStandardItemModel;
    setModel(model);
    mDefaultRootIndex = model->indexFromItem(model->invisibleRootItem());
}

void HPSTreeCombobox::setViewToTree(){
    qDebug() << Q_FUNC_INFO;

    mTreeView = new QTreeView();
    mTreeView->viewport()->installEventFilter(this);
    mTreeView->header()->hide();
    mTreeView->setMaximumHeight(400); // by default tree will show as 1 line high, need to reserve room
    mTreeView->setMinimumHeight(400);

    connect( mTreeView,SIGNAL(expanded(QModelIndex)),this,SLOT(saveExpandItem(QModelIndex)));
    connect( mTreeView,SIGNAL(collapsed(QModelIndex)),this,SLOT(removeExpandItem(QModelIndex)));
    setView(mTreeView);
    mListView = NULL;

}
void HPSTreeCombobox::setViewToList() {
    qDebug()<<Q_FUNC_INFO;

    mListView = new QListView();
    mListView->setMaximumHeight(400); // by default tree will show as 1 line high, need to reserve room
    mListView->setMinimumHeight(400);
    mListView->viewport()->installEventFilter(this);

    setView(mListView);
    mTreeView =NULL;
}

void HPSTreeCombobox::showPopup()
{

    setRootModelIndex(mDefaultRootIndex);
    QComboBox::showPopup();

    connect(lineEdit(), SIGNAL(textChanged(const QString &)),this , SLOT(blockLineEditChanged(const QString &)));
}

void HPSTreeCombobox::hidePopup()
{
    if (mSkipNextHide)
        mSkipNextHide = false;
    else
    {
        disconnect(lineEdit(), SIGNAL(textChanged(const QString &)),this , SLOT(blockLineEditChanged(const QString &)));
        updateText();
        QComboBox::hidePopup();

    }
}


void HPSTreeCombobox::blockLineEditChanged(const QString &)
{
    if (lineEdit()->text() != QDir::toNativeSeparators(mCIndex.data(Qt::UserRole).toString())){
        updateText();
    }
}


bool HPSTreeCombobox::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease||event->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent* m = static_cast<QMouseEvent*>(event);
        QModelIndex index = view()->indexAt(m->pos());
        QRect vrect = view()->visualRect(index);
        if (view()->rect().contains(m->pos())){

            mSkipNextHide = true;
        }
        if(event->type() == QEvent::MouseButtonPress  &&
                vrect.contains(m->pos()))

        {
            if(index.flags().testFlag(Qt::ItemIsEnabled)){
                mCIndex =index;
                mSkipNextHide = false;
                emit itemClicked(index);
                hidePopup();
            }
        }

    }
    return QComboBox::eventFilter(object, event);

}

void HPSTreeCombobox::updateText()
{
    if (mCIndex.isValid()) {
        lineEdit()->setText(QDir::toNativeSeparators(mCIndex.data(Qt::UserRole).toString()));
    } else {
        lineEdit()->setText("");
    }
}
void HPSTreeCombobox::setExpandedItems(const QList<QStandardItem *> &items)
{

    if (mTreeView!=NULL) {
        const int size = items.size();
        for (int var = 0; var < size; ++var) {
            mTreeView->expand( static_cast<QStandardItemModel *>(model())->indexFromItem(items.at(var)));
        }
    }
}
QStandardItemModel *HPSTreeCombobox::standardModel() const
{
    return static_cast<QStandardItemModel *>(model());
}

void HPSTreeCombobox::setCurrentItem(QStandardItem *item)
{
    QModelIndex index;
qDebug() << Q_FUNC_INFO <<  index << index.isValid();
    if(item != NULL){
        index = standardModel()->indexFromItem(item);
        view()->setCurrentIndex(index);
    }
    mCIndex = index;
    updateText();
}



const QString HPSTreeCombobox::getCurrentDir()
{
    return mCIndex.isValid()?mCIndex.data(Qt::UserRole).toString():QString("");
}

void HPSTreeCombobox::findeAndSetCurrentItem(const QString &dir)
{
    QStandardItem *item;
    item = findIndex( standardModel()->invisibleRootItem(),dir);
    qDebug() << item;
    if(item != NULL)
        setCurrentItem( item );
}

QStandardItem* HPSTreeCombobox::findIndex(QStandardItem *item,const QString &dir)
{
    QStandardItem *result;
    if(item->data(Qt::UserRole).toString() == dir)
        return item;
    const int countChildren = item->rowCount();

    for (int i = 0; i < countChildren; ++i) {
        result = findIndex(item->child(i),dir);
        if(result !=NULL)
            return result;
    }
    return NULL;
}

void HPSTreeCombobox::saveExpandItem(const QModelIndex &index)
{
    qDebug() << Q_FUNC_INFO << index.data(Qt::UserRole).toString();
    if(!mExpandeDirs.contains(index.data(Qt::UserRole).toString()))
        mExpandeDirs.append(index.data(Qt::UserRole).toString());
}

void HPSTreeCombobox::removeExpandItem(const QModelIndex &index)
{qDebug() << Q_FUNC_INFO << index.data(Qt::UserRole).toString();
    if(mExpandeDirs.contains(index.data(Qt::UserRole).toString()))
        mExpandeDirs.removeOne(index.data(Qt::UserRole).toString());
}
const QStringList & HPSTreeCombobox::expandeDirs()
{
    return mExpandeDirs;
}






