#ifndef HPSOPTIONWIDGET_H
#define HPSOPTIONWIDGET_H

#include <QtGui>
#include "hpsoption.h"

class HPSOptionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HPSOptionWidget(HPSOption *option,QWidget *parent = 0);
    void resetAndShow();
protected:
    void closeEvent ( QCloseEvent * event );
private:
    QLineEdit *eQuellOrdner;
    QPushButton *butQuellOrdner,*butUebernehmen,*butBeenden;
    QSlider *slider;
    QLabel *lSliderWert;
    QComboBox *comboBox;
    HPSOption *optionObject;
    QListWidget *listview;
    QList<QStandardItem*> dirItemList;
    int oldSliderValue;
    enum optionValues {
        QuellOrdner,PixSize,ComboBoxView,DirList
    };
    enum tabWidgets {
        Allgemein,Ordner
    };

    QVector<bool> options;
    void zuruecksetzten();
    void loadOrdner();
    QStringList checkRemovedDirs();
    void reinsertDirItems();

signals:
    void sliderTicked(int size);
    void scale();
    void comboBoxViewSelectedChanged(int);
    void dirsRemoved(const QStringList );
private slots:
    void comboBoxViewChanged(int);
    void quelleSuchen();
    void uebernehmen();
    void textchanged(const QString & text);
    void sliderChanged(int i);
    void cRemoveDir();

};



#endif // HPSOPTIONWIDGET_H
