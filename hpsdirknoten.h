#ifndef HPSDIRKNOTEN_H
#define HPSDIRKNOTEN_H
#include <QtCore>

class HPSDirKnoten {
public:
    HPSDirKnoten(): parent(NULL),dirname(""),_count(1)
    {}
    ~HPSDirKnoten(){}

    const HPSDirKnoten* getParent(){
        return this->parent;
    }
    void setParent( HPSDirKnoten *parent){
        this->parent =parent;
    }
    const QList<HPSDirKnoten*> & getChildren() const {
        return this->children;
    }
    void addChild(HPSDirKnoten * child){
        this->children.append(child);
    }
    void setDirname(const QString &dirName){
        this->dirname =dirName;
    }
    const QString &getDirname(){
        return this->dirname;
    }
    void removeChild( HPSDirKnoten *child){
        this->children.removeOne(child);
    }
    void resetChildren(){
        this->children.clear();
    }
    void setName(const QString &name){
        this->name = name;
    }
    const QString & getName(){
        return this->name;
    }
    void setActive(const bool active){
        this->active = active;
    }
    bool getActive() {
        return this->active;
    }
    quint8 count(){
        return _count;
    }
    void setCount(quint8 count){
        _count = count;
    }
    void dekrementCount(){
        --_count;
    }
    void inkrementCount() {
        ++_count;
    }

private:
    QString name;
    HPSDirKnoten *parent;
    QList<HPSDirKnoten*> children;
    QString dirname;
    bool active;
    quint8 _count;
};

#endif // HPSDIRKNOTEN_H
