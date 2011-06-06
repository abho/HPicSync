#include "hpsdirdialog.h"


HPSDirDialog::HPSDirDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    mModel.setFilter( QDir::AllDirs |QDir::NoDotAndDotDot);
     mTreeView->setModel( &mModel);
     QModelIndex index = mModel.setRootPath("");
     mTreeView->setRootIndex(index);
}

HPSDirDialog::~HPSDirDialog()
{

}

void HPSDirDialog::abbrechen()
{
    qDebug() << "muh";
    mDir = "";
    close();
}

void HPSDirDialog::auswahl()
{

    close();

}

QString HPSDirDialog::getDir()
{
    exec();
    return mDir;
}
