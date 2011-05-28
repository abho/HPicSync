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
//#include "hpstreecombobox_moc.cpp"

static const char g_ItemSep[] = "|";

//////////////////////////////////////////////////////////////////////////////////////////

/// hpstreecombobox

hpstreecombobox::hpstreecombobox(QModelIndex const defaultRootIdx, QWidget* parent) :
    QComboBox(parent), skipNextHide(false), treeView(NULL),listView(NULL),defaultRootIndex(defaultRootIdx)

{
    /*QTreeView* treeView = new QTreeView(this);
    treeView->header()->hide();
    treeView->setMaximumHeight(400); // by default tree will show as 1 line high, need to reserve room
    treeView->setMinimumHeight(400);
    setView(treeView);*/
    view()->setSelectionMode(QAbstractItemView::SingleSelection); // click to toggle items

    QComboBox::resize(200,200);
    setEditable(true);
    lineEdit()->setReadOnly(true);
    setInsertPolicy(QComboBox::NoInsert);
}

void hpstreecombobox::setViewToTree(){

    treeView = new QTreeView(this);
    treeView->viewport()->installEventFilter(this);
    treeView->header()->hide();
    treeView->setMaximumHeight(400); // by default tree will show as 1 line high, need to reserve room
    treeView->setMinimumHeight(400);


    qDebug() <<  treeView;
    setView(treeView);
}
void hpstreecombobox::setViewToList() {

    listView = new QListView(this);
    listView->setMaximumHeight(400); // by default tree will show as 1 line high, need to reserve room
    listView->setMinimumHeight(400);
    listView->viewport()->installEventFilter(this);

    qDebug() <<  listView;
    setView(listView);
}

void hpstreecombobox::showPopup()
{

    setRootModelIndex(defaultRootIndex);
    QComboBox::showPopup();
    //applyLineEditChanged();

    connect(lineEdit(), SIGNAL(textChanged(const QString &)), this, SLOT(blockLineEditChanged(const QString &)));
}

void hpstreecombobox::hidePopup()
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


void hpstreecombobox::blockLineEditChanged(const QString &)
{
    if (lineEdit()->text() != this->cIndex.data(Qt::UserRole).toString()){
        if(cIndex.isValid())
      lineEdit()->setText(this->cIndex.data(Qt::UserRole).toString());
        else
            lineEdit()->setText("");
    }

}
/*void hpstreecombobox::setEditText(const QString & text)
{Standardhpstreecombobox::
            QComboBox::setEditText(text);
}*/


bool hpstreecombobox::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease)
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
            this->cIndex =index;
            skipNextHide = false;
            this->hidePopup();
        }
    }
    return QComboBox::eventFilter(object, event);
}

void hpstreecombobox::updateText()
{
    if (cIndex.isValid()) {
        lineEdit()->setText(this->cIndex.data(Qt::UserRole).toString());
    } else {
        lineEdit()->setText("");
    }
}

///////////////////////////////////////////////////////////////////////////
// Standardhpstreecombobox

Standardhpstreecombobox::Standardhpstreecombobox(QWidget* parent /* = 0 */) : hpstreecombobox(QModelIndex(), parent)
{
    QStandardItemModel *model  = new QStandardItemModel;
    setModel(model);
    setRoot(model->indexFromItem(model->invisibleRootItem()));
}
QStandardItemModel *Standardhpstreecombobox::model() const
{
    return static_cast<QStandardItemModel *>(hpstreecombobox::model());
}

void Standardhpstreecombobox::addItem(const QString &text, const QVariant &userData)
{
    QStandardItem *item = new QStandardItem(text);
    item->setCheckable(false);
    item->setData(userData);
    model()->invisibleRootItem()->appendRow(item);
}

void Standardhpstreecombobox::setCurrentItem(QStandardItem *item)
{
    QModelIndex index;
    if(item != NULL){
        index = model()->indexFromItem(item);
        view()->setCurrentIndex(index);
    }
    cIndex = index;
    updateText();
}



const QString  Standardhpstreecombobox::getCurrentDir()
{
    return cIndex.isValid()?cIndex.data(Qt::UserRole).toString():QString("");
}

