#ifndef HPSPOPUPWIDGET_H
#define HPSPOPUPWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QDebug>
#include <QScrollBar>
#include <QCloseEvent>


namespace Ui {
    class HPSPopupWidget;
}

class HPSPopupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HPSPopupWidget(QWidget *parent = 0);
    ~HPSPopupWidget();
    QStandardItemModel * model();
    void showAndInit();
    bool eventFilter(QObject *, QEvent *);
    void setItem(QStandardItem *item);
    void setView(int view);
    void expanded(const QModelIndex &index);
    bool isListView();
    enum View{
        ListView,TreeView
    };

protected:
    void closeEvent(QCloseEvent *);
private slots:
    void on_mTreeView_expanded(const QModelIndex &index);
    void on_ScrollBar_RangeChanged(int,int);
    void on_mTreeView_activated(const QModelIndex &index);
    void on_mListView_activated(const QModelIndex &index);
    void on_mTreeView_collapsed(const QModelIndex &index);

signals:
    void exit();
    void itemClicked(QString);
private:
    Ui::HPSPopupWidget *ui;
    QStandardItemModel mModel;
    bool isVerticalOut;
    bool isHorizontalOut;
    Qt::MouseButtons mMousButtons;

    void initTreeView();
    void initListView();

};

#endif // HPSPOPUPWIDGET_H
