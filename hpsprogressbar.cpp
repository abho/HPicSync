#include "hpsprogressbar.h"

HPSProgressBar::HPSProgressBar(QWidget *parent) :
    QProgressBar(parent),mCountVisible(false)
{
}

void HPSProgressBar::paintEvent(QPaintEvent *event)
{
    QProgressBar::paintEvent(event);
    QRect region = event->rect();
    QPainter painter(this);
    QStyle *style = this->style();

    style->drawItemText(&painter,region,Qt::AlignCenter, palette(),true, text()+mCountString);

}

void HPSProgressBar::setMaxCount(const int count)
{

    mMaxCount = count;
    refreshCountString();
}

void HPSProgressBar::setCount(const int count)
{
    mCount = count;
    refreshCountString();

}

int HPSProgressBar::count()
{
    return mCount;
}

const int HPSProgressBar::maxCount()
{
    return mMaxCount;
}

void HPSProgressBar::setCountVisible(bool x)
{
    mCountVisible = x;
    refreshCountString();

}

void HPSProgressBar::refreshCountString()
{
    if(mCountVisible)
        mCountString = " ("+QString::number(mCount) +"/"+QString::number(mMaxCount)+")";
    else
        mCountString = "";

}
