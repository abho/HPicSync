#include "hpsdomdirmodel.h"

HPSDomDirModel::HPSDomDirModel(QObject *parent) :
    QObject(parent)
{
}

bool HPSDomDirModel::save( HPSKnotDirModel &model)
{
    qDebug() << Q_FUNC_INFO;
    QElapsedTimer timer;
    timer.start();
    QDomDocument doc( "DirModel" );
    QDomElement root = doc.createElement( "root" );
    doc.appendChild(root);

    addDomKnot(doc,root,model.root());

    QFile file( "test.xml" );
    if( !file.open( QIODevice::WriteOnly ) ){
        qDebug() <<"fehler";
        return false;
    }else {
        QTextStream ts( &file );
        ts << doc.toString();
        file.close();
        qDebug() << "test.xml" << "saved" << timer.elapsed();
        return true;
    }
}

void HPSDomDirModel::addDomKnot(QDomDocument &doc,QDomElement &e,DirKnot *knot)
{
    DirKnot *child;
    QDomElement c;
    QList<DirKnot*> &list = knot->children;
    const int size = list.size();
    for( int i = 0; i<size;++i){
        child = list.at(i);
        c = doc.createElement( "knot" );
        c.setAttribute("name",child->name);
        c.setAttribute("active",child->isActive);
        c.setAttribute("expanded",child->item->data(Qt::UserRole+1).toBool());
        c.setAttribute("path",child->path);
        addDomKnot(doc,c,child);
        e.appendChild(c);
    }
}

bool HPSDomDirModel::load(HPSKnotDirModel &model)
{
    QElapsedTimer timer;
    QDomDocument doc( "DirModel" );
    QFile file( "test.xml" );
    if( !file.open( QIODevice::ReadOnly ) ){
        qDebug() << Q_FUNC_INFO << file.fileName()<<"lies sich nicht oeffnen";
        return false;
    }
    if( !doc.setContent( &file ) )
    {
        file.close();
        qDebug() << Q_FUNC_INFO << "fehler";
        return false;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if( root.tagName() != "root" ){
        qDebug() << Q_FUNC_INFO<< "not root found";
        return false;
    }
    setDomKnot(root,model.root());
    qDebug() << Q_FUNC_INFO<<"moedlaufgefüllt"<<timer.elapsed();
    return true;
}

void HPSDomDirModel::setDomKnot(QDomElement &e, DirKnot *parent)
{
    QStandardItem *newItem;
    QDomElement element;
    DirKnot *child;
    const QDomNodeList &list = e.childNodes();
    const int size = list.size();
    for(  int i = 0 ;  i < size ; ++i ) {
        element = list.at(i).toElement();
        if(!element.isNull()){
            if( element.tagName() == "knot"){
                child = new DirKnot;
                child->name = element.attribute("name");
                child->isActive = element.attribute("active").toInt();
                child->path = element.attribute("path");
                newItem = new QStandardItem(child->name);
                newItem->setEnabled(child->isActive);
                child->item = newItem;
                child->item->setData(element.attribute("expanded").toInt(),Qt::UserRole+1);
                child->item->setToolTip(child->path);
                setDomKnot(element,child);

                parent->children.append(child);
                parent->item->appendRow(child->item);

            }
        }
    }

}
