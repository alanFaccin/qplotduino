
#ifndef CHANNEL_H
#define CHANNEL_H

#include "qcustomplot.h"

class Channel
{

public:
    Channel(    QCustomPlot *p );

    void        AddChannel( QPen pen );
    void        AddValue( double key, double value );
    void        ChangePen( QPen p );
    void        Clear       ( );

    void        Enable      ( );
    void        Disable     ( );
    bool        IsEnable    ( );

private:

    QCustomPlot     *customPlot;
    QCPGraph        *line,
                    *dot;

    bool            isEnable;

};

#endif // CHANNEL_H
