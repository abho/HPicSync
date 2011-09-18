#include "hpsoldlistdelegate.h"

#include <QStylePainter>
#include <QStyle>
#include <QApplication>
#include <QtCore>
#include <QMouseEvent>
#include <QStandardItemModel>

HPSOldListDelegate::HPSOldListDelegate(int size, QObject *parent) :QStyledItemDelegate(parent),mSize(size)
{
    qDebug() << Q_FUNC_INFO << size;
}
void HPSOldListDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option,
                                 const QModelIndex & index ) const{

    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);
    QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
    QRect r= option.rect;
    //   qDebug() << "nr:" << index.data(Qt::UserRole+1).toInt();

    // QImage *img = (QImage *)index.data(Qt::DecorationRole).value<void *>();


    const QPixmap &pix = index.data(Qt::DecorationRole).value<QPixmap>();
    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());
    QRect rec(r.left()+2,r.top()+2,r.width()-4,r.height()-22);


    style->drawItemPixmap(painter,rec,Qt::AlignHCenter,pix);
    style->drawItemText(painter,QRect(r.left()+2,r.top()+r.height()-20,
                                      r.width()-4,20),Qt::AlignLeft,opt.palette,true,index.data(Qt::DisplayRole).toString());

}

QWidget * HPSOldListDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const{
    return 0;

}
bool HPSOldListDelegate::editorEvent ( QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index ) {

    return false;
}

QSize HPSOldListDelegate::sizeHint( const QStyleOptionViewItem & option, const QModelIndex & index ) const{
    return QSize(mSize,mSize);
}
