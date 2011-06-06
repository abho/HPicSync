#ifndef HPSDIRDIALOG_H
#define HPSDIRDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QFileSystemModel>
#include "ui_hpsdirdialog.h"


class HPSDirDialog : public QDialog, private Ui::HPSDirDialog
{
    Q_OBJECT

public:
    explicit HPSDirDialog(QWidget *parent = 0);
    ~HPSDirDialog();
    QString getDir();
public slots:
    void abbrechen();
    void auswahl();
private:
    QString mDir;
    QFileSystemModel mModel;


};

#endif // HPSDIRDIALOG_H
