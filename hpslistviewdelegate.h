#ifndef HPSLISTVIEWDELEGATE_H
#define HPSLISTVIEWDELEGATE_H

#include <QItemDelegate>
#include <QDebug>

class HPSListViewDelegate : public QItemDelegate
{
public:
    HPSListViewDelegate(int size,bool *steck,QObject * parent = 0);
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QWidget *createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    bool editorEvent ( QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index ) ;
private:
    int size;
    bool *check;
};

#endif // HPSLISTVIEWDELEGATE_H
