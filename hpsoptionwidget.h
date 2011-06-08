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
    QLineEdit *mQuellOrdnerEditLine;
    QPushButton *mQuellOrdnerButton,*mUebernehmenButton,*mBeendenButton;
    QSlider *mSlider;
    QLabel *mSliderWertLabel;
    QComboBox *mComboBox;
    HPSOption *mOptionObject;
    QListWidget *mListWidget;
    QList<QListWidgetItem*> mDirItemList;
    int mOldSliderValue;
    QVector<bool> mOptions;

    enum optionValues {
        QuellOrdner,PixSize,ComboBoxView,DirList
    };
    enum tabWidgets {
        Allgemein,Ordner
    };

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
    void test();
};



#endif // HPSOPTIONWIDGET_H
