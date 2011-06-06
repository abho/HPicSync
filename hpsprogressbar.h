#ifndef HPSPROGRESSBAR_H
#define HPSPROGRESSBAR_H

#include <QProgressBar>
#include <QtCore>
#include <QtGui>
#include <QDebug>

class HPSProgressBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit HPSProgressBar(QWidget *parent = 0);
    void setCount(const int count);
    void setMaxCount(const int count);
     int count();
    const int maxCount();
    void setCountVisible(bool x);

protected:
    void paintEvent ( QPaintEvent * );

private:
    int mCount;
    int mMaxCount;
    bool mCountVisible;
    QString mCountString;

    void refreshCountString();

};

#endif // HPSPROGRESSBAR_H
