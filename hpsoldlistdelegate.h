#ifndef HPSOLDLISTDELEGATE_H
#define HPSOLDLISTDELEGATE_H


 #include <QStyledItemDelegate>
#include <QDebug>

class HPSOldListDelegate : public QStyledItemDelegate
{
public:
    HPSOldListDelegate(int size,QObject * parent = 0);
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QWidget *createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    bool editorEvent ( QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index ) ;
private:
    int mSize;
};

#endif // HPSOLDLISTDELEGATE_H
