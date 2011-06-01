#include "hpslistviewdelegate.h"

#include <QStylePainter>
#include <QStyle>
#include <QApplication>
#include <QtCore>
#include <QMouseEvent>
#include <QStandardItemModel>

HPSListViewDelegate::HPSListViewDelegate(int size, bool *check,QObject *parent) :QItemDelegate(parent),mSize(size),mCheck(check)
{
}
void HPSListViewDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option,
                          const QModelIndex & index ) const{
    QRect r= option.rect;
 //   qDebug() << "nr:" << index.data(Qt::UserRole+1).toInt();

    QImage *img = (QImage *)index.data(Qt::DecorationRole).value<void *>();

    if(mSize-5==img->size().width() || mSize-20==img->size().height()) {
   //     qDebug() << "nicht scalieren";
    } else {
       qDebug() << "scalieren";
        QImage *orginal = (QImage *)index.data(Qt::UserRole+2).value<void *>();
        *img=orginal->scaled(QSize(this->mSize-5,this->mSize-20),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    }
    QPixmap pix = QPixmap::fromImage(*img);
   // qDebug() << "pix size:" << pix.size();
    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());
    QRect rec(r.left()+5,r.top()+5,r.width()-5,r.height()-25);
    //qDebug() << "pix rec size" << rec;
    this->drawDecoration(painter,option,
                         rec,
                         pix);
    QStyleOptionViewItem op(option);
    op.font.setPointSize(8);
    op.fontMetrics= QFontMetrics(op.font);
    this->drawDisplay(painter,op,
                      QRect(r.left()+5,r.top()+r.height()-20,
                            r.width()-5,20),
                      index.data(Qt::DisplayRole).toString());
    Qt::CheckState state = static_cast<Qt::CheckState>(index.data(Qt::CheckStateRole).toUInt());

    this->drawCheck(painter,option,
                    QRect(r.left()+this->mSize-30,r.top(),
                          r.width()-this->mSize+30,r.height()-this->mSize+30),
                    state);
}

QWidget * HPSListViewDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const{
    return 0;

}
bool HPSListViewDelegate::editorEvent ( QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index ) {
    //qDebug() << "nmuh" << *this->check;
    if( event->type() == QEvent::MouseButtonRelease && !(*this->mCheck)){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if(mouseEvent->button()==Qt::LeftButton){
            qDebug()<< "MouseButtonLeftRelease";
            QRect re(option.rect.left()+this->mSize-30,option.rect.top(),
                     option.rect.width()-this->mSize+30,option.rect.height()-this->mSize+30);
            if(re.contains(mouseEvent->pos())){
                qDebug() << "getroffen";
                if(static_cast<Qt::CheckState>(model->data(index,Qt::CheckStateRole).toUInt())==Qt::Unchecked){
                    model->setData(index,Qt::Checked,Qt::CheckStateRole);
                } else{
                    model->setData(index,Qt::Unchecked,Qt::CheckStateRole);
                }
            }
        }
    } else if(event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if(mouseEvent->button()==Qt::LeftButton){
            qDebug()<< "MouseButtonLeftPress";
            QRect re(option.rect.left()+this->mSize-30,option.rect.top(),
                     option.rect.width()-this->mSize+30,option.rect.height()-this->mSize+30);
            if(re.contains(mouseEvent->pos())){
                qDebug() << "getroffen";
                *this->mCheck=false;
            }
        }
    }
    return false;
}

QSize HPSListViewDelegate::sizeHint( const QStyleOptionViewItem & option, const QModelIndex & index ) const{
    return QSize(this->mSize,this->mSize);
}

