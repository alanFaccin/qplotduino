
#include <QColorDialog>
#include <QColor>

#include "rxchannel.h"
#include "ui_rxchannel.h"

RxChannel::RxChannel( QWidget *parent ) :
                                QWidget(parent),
                                ui(new Ui::RxChannel)
{

    ui->setupUi(this);
    ui->chkChannelActive->setChecked( false );
    ui->lblColor->setAutoFillBackground( true );

}

RxChannel::~RxChannel()
{
    delete ui;
}

void RxChannel::SetChannel(int chId)
{

    m_channelId = chId;

    ui->grpChannelName->setTitle( "Canal " + QString::number( chId + 1 ) );

}

void RxChannel::SetPosition(int x, int y)
{
    this->setGeometry( x, y, this->width(), this->height() );
}

void RxChannel::SetChecked(bool status)
{

    ui->chkChannelActive->setChecked( status );

    if ( status )
        emit ActivateChannel( m_channelId );
    else
        emit DeactivateChannel( m_channelId );

}

void RxChannel::setLabelColor(QColor c)
{
    QPalette pal = ui->lblColor->palette();
    pal.setColor( QPalette::Window, c );
    ui->lblColor->setPalette(pal);
}

void RxChannel::on_btnChannelColor_clicked()
{

    QColor theColor = QColorDialog::getColor( Qt::white, this );

    if ( !theColor.isValid() )
        return;

    setLabelColor( theColor );


    emit ChangeColor( m_channelId, QPen( theColor ) );

}

void RxChannel::on_chkChannelActive_stateChanged(int arg1)
{


    if ( arg1 == Qt::Checked )
        SetChecked( true );
    else if ( arg1 == Qt::Unchecked )
        SetChecked( false );

}


