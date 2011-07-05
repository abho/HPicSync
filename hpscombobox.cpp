#include "hpscombobox.h"

HPSComboBox::HPSComboBox(QWidget *parent) :
    QComboBox(parent)
{
    mLineEdit = new QLineEdit();
    mLineEdit->setReadOnly(true);
    setLineEdit(mLineEdit);
    initPopup();
/*
    QStandardItem *p = new QStandardItem("root");
    p->setData("root",Qt::UserRole);
    mPopup->model()->appendRow(p);
    for (int var = 0; var < 20; ++var) {
        QStandardItem *child = new QStandardItem("muh "+QString::number(var)+p->text());
        child->setData("muh "+QString::number(var)+p->text(),Qt::UserRole);
        p->appendRow(child);
        if(var==3){
            child->setEnabled(false);
        }
        p =child;
    }
    mPopup->setView(HPSPopupWidget::TreeView);
    setItemByText("muh "+QString::number(0)+"root");*/
}

void HPSComboBox::showPopup()
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << geometry();
    QPoint p =mapToGlobal( QPoint(0,size().height()));
    qDebug() << p;
    QRect r(p.x(),p.y(),size().width(),200);
    mPopup->setGeometry(r);
    mPopup->showAndInit();
}

void HPSComboBox::initPopup()
{
    mPopup = new HPSPopupWidget(this);
    mPopup->setWindowFlags(Qt::Popup);
    connect(mPopup,SIGNAL(exit()),this,SLOT(onMPopupExit()));
    connect(mPopup,SIGNAL(itemClicked(QString)),this,SLOT(onMPopupItemClicked(QString)));
}
void HPSComboBox::onMPopupExit()
{
    hidePopup();
}

void HPSComboBox::onMPopupItemClicked(QString str)
{
    lineEdit()->setText(QDir::toNativeSeparators(str));
}

void HPSComboBox::setView(int view)
{
    mPopup->setView(view);
}

void HPSComboBox::setItemByText(const QString &str)
{
    qDebug() << Q_FUNC_INFO << str;
    QStandardItem *item;
    if(mPopup->isListView())
        item = findItemInList(str);
    else
        item = findItemInTree(str);

    qDebug() << item;
    if( item!= NULL){
        qDebug() << "gefunden";
        mLineEdit->setText(QDir::toNativeSeparators(str));
        mPopup->setItem(item);

    }else {
        mLineEdit->setText("");
    }
}


void HPSComboBox::loadExpanded()
{
    loadExpandedItem( mPopup->model()->invisibleRootItem());
}
void HPSComboBox::loadExpandedItem(QStandardItem *item)
{
    QStandardItem *child;
    qDebug() <<Q_FUNC_INFO << item->rowCount();
    if(item->hasChildren()){
        const int size = item->rowCount();
        for ( int i = 0 ; i < size ; ++i){
            child = item->child(i,0);
            if(child->data(Qt::UserRole+1).toBool()){
                mPopup->expanded( mPopup->model()->indexFromItem( child));
            }
            loadExpandedItem(child);
        }
    }
}

const QString  HPSComboBox::currentDir()
{
    return QDir::fromNativeSeparators(mLineEdit->text());
}

QStandardItem * HPSComboBox::findItemInList(const QString &dir)
{
    QStandardItem *item= NULL;
    QList<QStandardItem *> list = mPopup->model()->findItems(QDir::toNativeSeparators(dir));
    if(!list.isEmpty())
        item = list.first();
    return item;
}

QStandardItem * HPSComboBox::findItemInTree(const QString &dir)
{
    qDebug() << Q_FUNC_INFO;
    QStandardItem *parent,*child=NULL;
    parent = mPopup->model()->invisibleRootItem();
    const QStringList list =dir.split("/",QString::SkipEmptyParts);
    const int size = list.size();
    int countChildren;
    bool gefunden;
    for (int i = 0; i < size; ++i) {
        gefunden =false;
        countChildren = parent->rowCount();
        for (int j = 0; j < countChildren; ++j) {
            child = parent->child(j,0);
            if(child->text()==list.at(i)){
                parent = child;
                gefunden = true;
                break;
            }
        }
        if(!gefunden)
            return NULL;
    }
    return child;
}

QStandardItemModel * HPSComboBox::standardModel()
{
    return mPopup->model();
}

void HPSComboBox::clearSelection()
{
    mLineEdit->setText("");
    mPopup->clearSelection();
}
