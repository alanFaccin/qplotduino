
#include "channel.h"

Channel::Channel(QCustomPlot *p)
{

    customPlot  =   p;
    isEnable    =   false;

}

void Channel::AddChannel(   QPen pen )
{

    line = customPlot->addGraph(); // blue line
    line->setPen( pen );
///  line->setBrush(QBrush(QColor(240, 255, 200)));
    line->setAntialiasedFill(false);

    dot = customPlot->addGraph(); // blue dot
    dot->setPen( pen );
    dot->setLineStyle(QCPGraph::lsNone);
    dot->setScatterStyle( QCPScatterStyle::ssDisc );

}

void Channel::AddValue(double key, double value)
{

    line->addData( key, value );

    dot->clearData();
    dot->addData( key, value );

    // remove data of lines that's outside visible range:
    // rescale value (vertical) axis to fit the current data:
    line->removeDataBefore( key-8 );
    line->rescaleValueAxis( );

}

void Channel::ChangePen(QPen p)
{

    line->setPen( p );
    dot->setPen( p );

}

void Channel::Clear()
{
    line->clearData();
    dot->clearData();

}

void Channel::Enable()
{
    isEnable    =   true;
}

void Channel::Disable()
{
    isEnable    =   false;
}

bool Channel::IsEnable()
{
    return isEnable;
}


