#include "hpsworkerclass.h"

bool HPSWorkerClass::mEx = false;

HPSWorkerClass::HPSWorkerClass(QObject *parent) :
    QObject(parent)
{
}

void HPSWorkerClass::beenden()
{
    qDebug() << Q_FUNC_INFO << "beenden";
    mEx = true;
}
