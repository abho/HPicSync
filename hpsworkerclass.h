#ifndef HPSWORKERCLASS_H
#define HPSWORKERCLASS_H

#include <QObject>
#include <QDebug>

class HPSWorkerClass : public QObject
{
    Q_OBJECT
public:
    explicit HPSWorkerClass(QObject *parent = 0);
    static void beenden();

protected:
   static bool mEx;



};

#endif // HPSWORKERCLASS_H
