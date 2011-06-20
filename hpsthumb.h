#ifndef HPSTHUMB_H
#define HPSTHUMB_H

#include<QString>
#include<QImage>

class HPSThumb
{
public:
    HPSThumb();
    HPSThumb(const QString &n);
    ~HPSThumb();
    QString name;
    QString hash;
    QString lastModi;
    int size;
    QImage image;
    bool error;

};

#endif // HPSTHUMB_H
