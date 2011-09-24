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
    const int count();
    void setText(QString str);

protected:
    void paintEvent ( QPaintEvent * );

private:
    int mCount;
    QString mText;

};

#endif // HPSPROGRESSBAR_H
