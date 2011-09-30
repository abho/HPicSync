#include "hpspopupwidget.h"
#include "ui_hpspopupwidget.h"

HPSPopupWidget::HPSPopupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HPSPopupWidget),isVerticalOut(false),isHorizontalOut(false)
{
    ui->setupUi(this);
    initTreeView();
    initListView();
    connect(ui->mTreeView->verticalScrollBar(),SIGNAL(sliderReleased()),this,SLOT(findVisibleIndex()));

}

HPSPopupWidget::~HPSPopupWidget()
{
    delete ui;
}

QStandardItemModel * HPSPopupWidget::model()
{
    return &mModel;
}

void HPSPopupWidget::on_mTreeView_expanded(const QModelIndex &index)
{

    mModel.itemFromIndex(index)->setData(true,Qt::UserRole+1);
    QRect r =ui->mTreeView->visualRect(index);
    if(r.y()+r.height() > size().height()-40)
        isVerticalOut = true;
    else
        isVerticalOut = false;


}

void HPSPopupWidget::on_ScrollBar_RangeChanged(int min,int max)
{

    QScrollBar *bar = (QScrollBar*)sender();
    if( bar == ui->mTreeView->horizontalScrollBar()){
        if(isHorizontalOut)
            bar->setValue(max);
    } else {
        if(isVerticalOut)
            bar->setValue(max);
    }
}

void HPSPopupWidget::closeEvent(QCloseEvent *event)
{

    emit exit();
    event->accept();
}

bool HPSPopupWidget::eventFilter(QObject *o, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick || event->type() == QEvent::MouseButtonRelease){

        QMouseEvent* m = static_cast<QMouseEvent*>(event);
        QModelIndex index = ui->mTreeView->indexAt(m->pos());

        QRect vrect = ui->mTreeView->visualRect(index);
        if(event->type() == QEvent::MouseButtonPress  &&
                vrect.contains(m->pos()))
        {
            if(index.flags().testFlag(Qt::ItemIsEnabled)){
                emit itemClicked(index.data(Qt::UserRole).toString());
                hide();
                emit exit();
            }
        }
    }
    return false;
}

void HPSPopupWidget::setItem(QStandardItem *item)
{
    if(ui->mListView->isHidden()){
        ui->mTreeView->setCurrentIndex(mModel.indexFromItem(item));
    }else {
        ui->mListView->setCurrentIndex(mModel.indexFromItem(item));
    }
}


void HPSPopupWidget::on_mTreeView_activated(const QModelIndex &index)
{
    qDebug() << Q_FUNC_INFO << index.data(Qt::UserRole).toString() << index.flags().testFlag(Qt::ItemIsEnabled);
    QRect vrect = ui->mTreeView->visualRect(index);
    if( index.flags().testFlag(Qt::ItemIsEnabled)&&vrect.contains(mapFromGlobal(QCursor::pos()))){
        qDebug() << "ecit";
        emit itemClicked(index.data(Qt::UserRole).toString());
        hide();
        emit exit();
    }

}

void HPSPopupWidget::on_mListView_activated(const QModelIndex &index)
{
    qDebug() << Q_FUNC_INFO << index.data(Qt::UserRole).toString();
    emit itemClicked(index.data(Qt::UserRole).toString());
    hide();
    emit exit();
}

void HPSPopupWidget::initTreeView()
{

    ui->mTreeView->setTextElideMode(Qt::ElideNone);
    ui->mTreeView->header()->setResizeMode(QHeaderView::ResizeToContents);
    ui->mTreeView->viewport()->installEventFilter(this);

    connect(ui->mTreeView->verticalScrollBar(),SIGNAL(rangeChanged(int,int)),this,SLOT(on_ScrollBar_RangeChanged(int,int)));
    connect(ui->mTreeView->horizontalScrollBar(),SIGNAL(rangeChanged(int,int)),this,SLOT(on_ScrollBar_RangeChanged(int,int)));
    ui->mTreeView->hide();
}

void HPSPopupWidget::initListView()
{
    ui->mListView->hide();
}

void HPSPopupWidget::setView(int view)
{
    if( view == ListView){
        ui->mTreeView->hide();
        ui->mListView->setModel(&mModel);
        ui->mListView->show();
    }else if(view == TreeView ) {
        ui->mListView->hide();
        ui->mTreeView->setModel(&mModel);
        ui->mTreeView->show();
    }
}

void HPSPopupWidget::showAndInit()
{
    show();
    if( !ui->mListView->isHidden()){
        ui->mListView->viewport()->setFocus();
    }else{
        ui->mTreeView->viewport()->setFocus();

        findVisibleIndex();
    }


}

void HPSPopupWidget::expanded(const QModelIndex &index)
{
    ui->mTreeView->expand(index);
}

void HPSPopupWidget::on_mTreeView_collapsed(const QModelIndex &index)
{
    mModel.itemFromIndex(index)->setData(false,Qt::UserRole+1);
}

bool HPSPopupWidget::isListView()
{
    return !ui->mListView->isHidden();
}

void HPSPopupWidget::on_mListView_clicked(const QModelIndex &index)
{
    emit itemClicked(index.data(Qt::UserRole).toString());
    hide();
    emit exit();
}

void HPSPopupWidget::clearSelection()
{ if( ui->mListView->isHidden()){
        ui->mTreeView->selectionModel()->clearSelection();
    }else{
        ui->mListView->selectionModel()->clearSelection();
    }
}

void HPSPopupWidget::findVisibleIndex()
{

    QModelIndex last=   ui->mTreeView->indexAt(ui->mTreeView->viewport()->rect().bottomLeft());
    QModelIndex tmp =ui->mTreeView->indexAt(ui->mTreeView->viewport()->rect().topLeft());
    while(tmp.isValid()){
        qDebug() << tmp.data(Qt::UserRole).toString();
        qDebug() << "muh" <<  QDir(tmp.data(Qt::UserRole).toString()).entryList(QStringList() << "*.jpg" << "*.png").size();
        tmp = ui->mTreeView->indexBelow(tmp);
        if(tmp == last){
            qDebug() << tmp.data(Qt::UserRole).toString();
            qDebug() << "muh" <<  QDir(tmp.data(Qt::UserRole).toString()).entryList(QStringList() << "*.jpg" << "*.png").size();
            break;
        }
    }
}


