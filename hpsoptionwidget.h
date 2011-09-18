#ifndef HPSOPTIONWIDGET_H
#define HPSOPTIONWIDGET_H

#include <QtGui>
#include "hpsoption.h"
#include "ui_hpsoptionwidget.h"

class HPSOptionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HPSOptionWidget(HPSOption *option,QWidget *parent = 0);
    ~HPSOptionWidget();
    void resetAndShow();
protected:
    void closeEvent ( QCloseEvent * event );
private:
    Ui::HPSOptionWidget *ui;

    HPSOption *mOptionObject;
    int mOldSliderValue;
    QVector<bool> mOptions;

    enum optionValues {
       PixSize,ComboBoxView
    };
    enum tabWidgets {
        Allgemein,Ordner
    };

    void zuruecksetzten();
    QStringList checkRemovedDirs();
    void reinsertDirItems();

signals:
    void sliderTicked(int size);
    void comboBoxViewSelectedChanged(int);
private slots:
    void on_sliderThumbSize_valueChanged(int value);
    void on_comboBoxDirsView_currentIndexChanged(int index);
    void on_butOk_clicked();
    void on_butCancel_clicked();
};



#endif // HPSOPTIONWIDGET_H
