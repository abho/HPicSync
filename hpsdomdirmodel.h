#ifndef HPSDOMDIRMODEL_H
#define HPSDOMDIRMODEL_H

#include <QObject>
#include "hpsknotdirmodel.h"
#include <QDomDocument>
#include <QtCore>
class HPSDomDirModel : public QObject
{
    Q_OBJECT
public:
    explicit HPSDomDirModel(QObject *parent = 0);
    bool save( HPSKnotDirModel & model);
    bool load( HPSKnotDirModel & model);

signals:

public slots:
private:
    void addDomKnot( QDomDocument &doc,QDomElement &e,DirKnot *knot);
    void setDomKnot(QDomElement &e,DirKnot *parent);
};

#endif // HPSDOMDIRMODEL_H
