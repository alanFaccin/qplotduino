#ifndef RXCHANNEL_H
#define RXCHANNEL_H

#include <QWidget>

#include <QPen>

namespace Ui {
    class RxChannel;
}

class RxChannel : public QWidget
{
    Q_OBJECT

public:
    RxChannel ( QWidget *parent = 0 );
    ~RxChannel();

    void        SetChannel      ( int chId );
    void        SetPosition     ( int x, int y );
    void        SetChecked      ( bool status );
    void        setLabelColor   ( QColor c );

private slots:
    void on_btnChannelColor_clicked();
    void on_chkChannelActive_stateChanged(int arg1);


signals:
    void    ActivateChannel     ( int chId );
    void    DeactivateChannel   ( int chId );
    void    ChangeColor         ( int chId, QPen pen );

private:
    Ui::RxChannel *ui;

    int     m_channelId;

};

#endif // RXCHANNEL_H
