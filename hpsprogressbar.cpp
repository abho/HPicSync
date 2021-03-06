#include "hpsprogressbar.h"

HPSProgressBar::HPSProgressBar(QWidget *parent) :
    QProgressBar(parent)
{
}

void HPSProgressBar::paintEvent(QPaintEvent *event)
{
    QProgressBar::paintEvent(event);
    QRect region = event->rect();
    QPainter painter(this);
    QStyle *style = this->style();
    style->drawItemText(&painter,region,Qt::AlignCenter, palette(),true, mText);

}



void HPSProgressBar::setCount(const int count)
{
    mCount = count;

}

const int HPSProgressBar::count()
{
    return mCount;
}

void HPSProgressBar::setText(QString str)
{
    mText = str;
}






