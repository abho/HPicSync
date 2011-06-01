//////////////////////////////////////////////////////////////////////////////////////////
// Code to create a QComboBox where multiple items can be selected and the items
// are shown in a tree.
// Written by Corwin Joy
// This code is hereby placed in the Public Domain.
// This code comes with no warranty of any kind, use at your own risk.qwe c

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
//#include "hpstreecombobox_moc.cpp"

static const char g_ItemSep[] = "|";

//////////////////////////////////////////////////////////////////////////////////////////

/// hpstreecombobox

HPSTreeCombobox::HPSTreeCombobox(QModelIndex const defaultRootIdx, QWidget* parent) :
    QComboBox(parent), skipNextHide(false), treeView(NULL),listView(NULL),defaultRootIndex(defaultRootIdx)

{
    /*QTreeView* treeView = new QTreeView(this);
    treeView->header()->hide();
    treeView->setMaximumHeight(400); // by default tree will show as 1 line high, need to reserve room
    treeView->setMinimumHeight(400);
    setView(treeView);*/
    view()->setSelectionMode(QAbstractItemView::SingleSelection); // click to toggle items

    //QComboBox::resize(200,200);
    setEditable(true);
    lineEdit()->setReadOnly(true);
    setInsertPolicy(QComboBox::NoInsert);
}

void HPSTreeCombobox::setViewToTree(){
    qDebug() << Q_FUNC_INFO;

    treeView = new QTreeView(this);
    treeView->viewport()->installEventFilter(this);
    treeView->header()->hide();
    treeView->setMaximumHeight(400); // by default tree will show as 1 line high, need to reserve room
    treeView->setMinimumHeight(400);
    connect( treeView,SIGNAL(clicked(QModelIndex)),this,SLOT(itemClicked(QModelIndex)));
    connect( treeView,SIGNAL(expanded(QModelIndex)),this,SLOT(saveExpandItem(QModelIndex)));
    connect( treeView,SIGNAL(collapsed(QModelIndex)),this,SLOT(removeExpandItem(QModelIndex)));
    setView(treeView);
    listView = NULL;

}
void HPSTreeCombobox::setViewToList() {
    qDebug()<<Q_FUNC_INFO;

    listView = new QListView(this);
    listView->setMaximumHeight(400); // by default tree will show as 1 line high, need to reserve room
    listView->setMinimumHeight(400);
    listView->viewport()->installEventFilter(this);
    connect( listView,SIGNAL(clicked(QModelIndex)),this,SLOT(itemClicked(QModelIndex)));

    treeView->disconnect();
    setView(listView);
    treeView =NULL;
}

void HPSTreeCombobox::showPopup()
{

    setRootModelIndex(defaultRootIndex);
    QComboBox::showPopup();
    //applyLineEditChanged();

    connect(lineEdit(), SIGNAL(textChanged(const QString &)), this, SLOT(blockLineEditChanged(const QString &)));
}

void HPSTreeCombobox::hidePopup()
{
    if (skipNextHide)
        skipNextHide = false;
    else
    {
        disconnect(lineEdit(), SIGNAL(textChanged(const QString &)), this, SLOT(blockLineEditChanged(const QString &)));
        this->updateText();
        QComboBox::hidePopup();

    }
}


void HPSTreeCombobox::blockLineEditChanged(const QString &)
{
    if (lineEdit()->text() != QDir::toNativeSeparators(this->cIndex.data(Qt::UserRole).toString())){
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

            skipNextHide = true;
        }
        if(event->type() == QEvent::MouseButtonPress  &&
                vrect.contains(m->pos()))

        {
            if(index.flags().testFlag(Qt::ItemIsEnabled)){

                this->cIndex =index;
                skipNextHide = false;
                this->hidePopup();
            }
        }

    }
    return QComboBox::eventFilter(object, event);

}

void HPSTreeCombobox::updateText()
{
    if (cIndex.isValid()) {
        lineEdit()->setText(QDir::toNativeSeparators(this->cIndex.data(Qt::UserRole).toString()));
    } else {
        lineEdit()->setText("");
    }
}
void HPSTreeCombobox::setExpandedItems(const QList<QStandardItem *> &items)
{

    if (treeView!=NULL) {
        const int size = items.size();
        for (int var = 0; var < size; ++var) {
            treeView->expand( static_cast<QStandardItemModel *>(model())->indexFromItem(items.at(var)));
        }
    }
}
///////////////////////////////////////////////////////////////////////////
// Standardhpstreecombobox

StandardHPSTreeCombobox::StandardHPSTreeCombobox(QWidget* parent /* = 0 */) : HPSTreeCombobox(QModelIndex(), parent)
{
    QStandardItemModel *model  = new QStandardItemModel;
    setModel(model);
    setRoot(model->indexFromItem(model->invisibleRootItem()));
}
QStandardItemModel *StandardHPSTreeCombobox::model() const
{
    return static_cast<QStandardItemModel *>(HPSTreeCombobox::model());
}



void StandardHPSTreeCombobox::setCurrentItem(QStandardItem *item)
{
    QModelIndex index;
    if(item != NULL){
        index = model()->indexFromItem(item);
        view()->setCurrentIndex(index);
    }
    cIndex = index;
    updateText();
}



const QString  StandardHPSTreeCombobox::getCurrentDir()
{
    return cIndex.isValid()?cIndex.data(Qt::UserRole).toString():QString("");
}

void StandardHPSTreeCombobox::findeAndSetCurrentItem(const QString &dir)
{
    QStandardItem *item;
    item = findIndex( model()->invisibleRootItem(),dir);
    qDebug() << item;
    if(item != NULL)
        setCurrentItem( item );
}

QStandardItem* StandardHPSTreeCombobox::findIndex(QStandardItem *item,const QString &dir)
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

void HPSTreeCombobox::itemClicked(QModelIndex index)
{
    //   qDebug() << index.data(Qt::UserRole) << "geklickt";
}

const QStringList &StandardHPSTreeCombobox::expandeDirs()
{
    return expandeDirs_;
}

void HPSTreeCombobox::saveExpandItem(const QModelIndex &index)
{
    qDebug() << Q_FUNC_INFO << index.data(Qt::UserRole).toString();
    if(!expandeDirs_.contains(index.data(Qt::UserRole).toString()))
        expandeDirs_.append(index.data(Qt::UserRole).toString());
}

void HPSTreeCombobox::removeExpandItem(const QModelIndex &index)
{qDebug() << Q_FUNC_INFO << index.data(Qt::UserRole).toString();
    if(expandeDirs_.contains(index.data(Qt::UserRole).toString()))
        expandeDirs_.removeOne(index.data(Qt::UserRole).toString());
}





