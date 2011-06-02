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
    QImage image;
    bool error;

};

#endif // HPSTHUMB_H
