#ifndef HPSDIRMANAGER_H
#define HPSDIRMANAGER_H

#include <QObject>
#include <QStandardItemModel>
#include "hpstreecombobox.h"
#include "hpsoption.h"
class HPSDirManager : public QObject
{
    Q_OBJECT
public:
    explicit HPSDirManager(HPSOption &option,QObject *parent = 0);
    ~HPSDirManager();
    void addDir(const QString  &dir);
    void setModel( QStandardItemModel *model);
    QList<QStandardItem*> makeView();
    //void removeDirs(QStringList dirs);

    const QStringList & dirs();

private:
    void addDirToTree(QList<QStandardItem*> & expandedItems,const QString &dir);
    void addDirToTree(const QString &dir);
    void addDirToList(const QString &dir);
    void removeDirFromList(const QString &dir);
void removeDirFromTree(const QString &dir);
    HPSOption &mOption;
    QStandardItemModel *currentModel;


signals:

public slots:
    void removeDirs(QStringList dirs);
};

#endif // HPSDIRMANAGER_H
