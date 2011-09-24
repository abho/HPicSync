#include "hpsthumbmanager.h"

HPSThumbManager::HPSThumbManager(HPSDBHandler &handler,HPSOption &option, QObject *parent) :
    QObject(parent), mOption(option),mDatabaseHandler(handler)
{
}
void HPSThumbManager::fertigTime()
{
   // qDebug() << "fertig" << mTimer.elapsed();

}


void HPSThumbManager::setListWidget(QListWidget *listWidget)
{
    mListWidget = listWidget;
}
void HPSThumbManager::loadThumbs(const QString &dir)
{
    qDebug() << Q_FUNC_INFO << dir;

        mListWidget->clear();
        QList<QStringList> list;
        list = mDatabaseHandler.hashPaths(dir);
        makeView(list);

}
void HPSThumbManager::makeView(const QList<QStringList> &list)
{
    const QStringList &hashList = list.at(0);
    const QStringList &nameList = list.at(1);
    const int size = hashList.size();
    QPixmap pix;
    for (int var = 0; var < size; ++var) {
        QListWidgetItem *item= new QListWidgetItem();
        if(pix.load(QApplication::applicationDirPath()+"/.thumbs/"+hashList.at(var)+".png")){

            item->setData(Qt::DisplayRole,nameList.at(var));
            item->setData(Qt::DecorationRole,pix);
        } else {
            qDebug() << "fehler bei laden von "<<QApplication::applicationDirPath()+"/.thumbs/"+hashList.at(var)+".jpg";
        }
        //img =&this->fotos[i];
        mListWidget->addItem(item);
    }
}





