#include "hpsoldlistdelegate.h"

#include <QStylePainter>
#include <QStyle>
#include <QApplication>
#include <QtCore>
#include <QMouseEvent>
#include <QStandardItemModel>

HPSOldListDelegate::HPSOldListDelegate(int size, QObject *parent) :QStyledItemDelegate(parent),mSize(size)
{
}
void HPSOldListDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option,
                                 const QModelIndex & index ) const{

    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);
    QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
    QRect r= option.rect;
    //   qDebug() << "nr:" << index.data(Qt::UserRole+1).toInt();

    QImage *img = (QImage *)index.data(Qt::DecorationRole).value<void *>();


    if(mSize-5==img->size().width() || mSize-20==img->size().height()) {
        //     qDebug() << "nicht scalieren";
    } else {
        qDebug() << "scalieren" << index.data(Qt::UserRole+1).toInt();
        QImage *orginal = (QImage *)index.data(Qt::UserRole+2).value<void *>();
        *img=orginal->scaled(QSize(mSize-5,mSize-20),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    }
    QPixmap pix = QPixmap::fromImage(*img);
    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());
    QRect rec(r.left()+5,r.top()+5,r.width()-5,r.height()-25);


    style->drawItemPixmap(painter,rec,Qt::AlignHCenter,pix);
    style->drawItemText(painter,QRect(r.left()+5,r.top()+r.height()-20,
                                      r.width()-5,20),Qt::AlignHCenter,opt.palette,true,index.data(Qt::DisplayRole).toString());

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
