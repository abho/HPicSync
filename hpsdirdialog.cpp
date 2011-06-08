#include "hpsdirdialog.h"


HPSDirDialog::HPSDirDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    mModel.setFilter( QDir::AllDirs |QDir::NoDotAndDotDot);

     mTreeView->setModel( &mModel);
     QHeaderView *header =mTreeView->header();
     header->setSectionHidden(1,true);
     header->setSectionHidden(2,true);
     header->setSectionHidden(3,true);
     QModelIndex index = mModel.setRootPath(QApplication::applicationDirPath());
     mTreeView->setRootIndex(mModel.index(QDir::rootPath()));
     mTreeView->setCurrentIndex(index);
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
