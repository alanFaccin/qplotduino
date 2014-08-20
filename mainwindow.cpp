#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{

    ui->setupUi(this);


    /////////////////////////////////////
    ///
    ///
    ///
    h_WidgetChannel1 = AddNewChannelWidget( 0, 780, 91 );
    h_WidgetChannel1->SetPosition( 780, 91 );

    h_WidgetChannel2 = AddNewChannelWidget( 1, 780, 170 );
    h_WidgetChannel3 = AddNewChannelWidget( 2, 780, 250 );
    h_WidgetChannel4 = AddNewChannelWidget( 3, 780, 330 );
    h_WidgetChannel5 = AddNewChannelWidget( 4, 780, 410 );


    //////////////////////////////////////
    ///
    ///
    ///
    dwYRange    =   2.0;

    h_Channel1  =   new Channel( ui->customPlot );
    h_Channel2  =   new Channel( ui->customPlot );
    h_Channel3  =   new Channel( ui->customPlot );
    h_Channel4  =   new Channel( ui->customPlot );
    h_Channel5  =   new Channel( ui->customPlot );


    h_Channel1->AddChannel( QPen(Qt::blue) );       h_WidgetChannel1->setLabelColor( Qt::blue );
    h_Channel2->AddChannel( QPen(Qt::red ) );       h_WidgetChannel2->setLabelColor( Qt::red );
    h_Channel3->AddChannel( QPen(Qt::yellow ) );    h_WidgetChannel3->setLabelColor( Qt::yellow );
    h_Channel4->AddChannel( QPen(Qt::black ) );     h_WidgetChannel4->setLabelColor( Qt::black );
    h_Channel5->AddChannel( QPen(Qt::green ) );     h_WidgetChannel5->setLabelColor( Qt::green );

    dwGrid      =   10.0;
    ui->sliderGrid->setValue( 10 );

    ui->customPlot->xAxis->setAutoTickStep( false );
    ui->customPlot->yAxis->setAutoTickStep( false );
    ui->customPlot->xAxis->setTickStep( dwGrid );
    ui->customPlot->axisRect()->setupFullAxesBox( );

    ///
    /// Status bar
    ///
    dwPktCount          =   0;
    dwErrorCount        =   0;
    h_lblSerialPort     =   new QLabel( "Porta fechada" );
    h_lblMsgCount       =   new QLabel( "0" );
    h_lblErrors         =   new QLabel( "Erros: 0" );
    h_lblActiveTime     =   new QLabel( "00:00:00" );

    h_lblSerialPort->setFixedWidth( 200 );
    h_lblMsgCount->setFixedWidth( 100 );
    h_lblErrors->setFixedWidth( 100 );
    h_lblActiveTime->setFixedWidth( 100 );

    ui->statusBar->addWidget( h_lblSerialPort );
    ui->statusBar->addWidget( h_lblMsgCount );
    ui->statusBar->addWidget( h_lblErrors );
    ui->statusBar->addWidget( h_lblActiveTime );

    //////////////////////////////////////////////////////////////////////////////
    ///
    ///
    ui->sliderAmplitude->setValue( 20 ); /// 2.0

    ///
    /// make left and bottom axes transfer their ranges to right and top axes:
    ///
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    ///
    ///
    ///
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer.start( 100 );
    ui->sliderTimer->setValue( 100 );

    ///
    ///
    ///
    connect(&tmrCaptureCountTime, SIGNAL(timeout()), this, SLOT(runningTime()));


    ////////////////////////////
    ///
    ///
    h_RS    =   new SerialCommunication( this );

    connect( h_RS, SIGNAL(DataArrive(QString)), this, SLOT(SerialDataArrive(QString)) );
    h_RS->PopulateSerialList();
    QStringList     serialList = h_RS->getSerialList();

    int i = 0;
    ui->cmbSerialList->clear();
    while ( i != serialList.count() )
    {

        ui->cmbSerialList->addItem( serialList.at( i ) );
        i++;

    }

    h_lblSerialPort->setText( "Porta serial fechada" );


    dwDivisor    =  0;
    ui->sliderDivisor->setValue( 0 );

}

MainWindow::~MainWindow()
{

    delete ui;

}


RxChannel *MainWindow::AddNewChannelWidget( int chId, int x, int y )
{

    RxChannel       *rCh = new RxChannel( this );

    rCh->SetChannel( 0 );
    rCh->SetPosition( 780, 91 );
    rCh->SetChannel( chId );
    rCh->SetPosition( x, y );

    connect( rCh, SIGNAL(ActivateChannel(int)), this, SLOT(ActivateChannel(int)) );
    connect( rCh, SIGNAL(DeactivateChannel(int)), this, SLOT( DeactivateChannel(int)) );
    connect( rCh, SIGNAL(ChangeColor(int,QPen)), this, SLOT(ChangeChannelColor(int,QPen)) );

    return rCh;
}


///
/// Atualiza o grafico (mesmo se nao houver dados)
///
void MainWindow::realtimeDataSlot()
{

    ui->customPlot->xAxis->setRange(   dataX + 0.25, (dataX+dwYRange*2), Qt::AlignCenter );
    ui->customPlot->yAxis->setRange(   dwYRange * -1, dwYRange );

    ui->customPlot->xAxis->setTickStep( dwGrid );
    ui->customPlot->yAxis->setTickStep( dwGrid );

    ui->customPlot->xAxis->setScaleRatio( ui->customPlot->yAxis, 1.0 );
    ui->customPlot->replot();

}

void MainWindow::runningTime()
{

    ///
    /// Exibe tempo de execução da captura
    /// \TODO Usar QT
    ///
    dwCaptureSecs++;

    int seconds = 0,
        minutes = 0;

    unsigned long work = dwCaptureSecs;

    seconds = work % 60;
    work /= 60;

    minutes = work % 60;
    work /= 60;

    h_lblActiveTime->setText( QTime( work, minutes, seconds).toString( "hh:mm:ss" ) );

}


void MainWindow::SerialDataArrive(QString data)
{

    ///<millis>;<sin(millis)
    QStringList pData = data.split( ';' );

    if ( pData.size()  != 5 )
    {

        /// Incrementa contador de erros
        dwErrorCount++;
        h_lblErrors->setText( "Erros: " + QString::number( dwErrorCount ) );
        return;

    }

    ///
    ///

    int                 t       =   timeLapse.elapsed();

    dataX   =     (double)(t / 1000 );
    dataX   +=  ( (double)(t % 1000) ) / 1000;


    double  dataCh0   =   pData.at(0).toDouble( ),
            dataCh1   =   pData.at(1).toDouble( ),
            dataCh2   =   pData.at(2).toDouble( ),
            dataCh3   =   pData.at(3).toDouble( ),
            dataCh4   =   pData.at(4).toDouble( );

    if ( dwDivisor != 0 )
    {
        dataCh0 /= dwDivisor;
        dataCh1 /= dwDivisor;
        dataCh2 /= dwDivisor;
        dataCh3 /= dwDivisor;
        dataCh4 /= dwDivisor;
    }

    if ( h_Channel1->IsEnable() )   h_Channel1->AddValue( dataX, dataCh0 );
    if ( h_Channel2->IsEnable() )   h_Channel2->AddValue( dataX, dataCh1 );
    if ( h_Channel3->IsEnable() )   h_Channel3->AddValue( dataX, dataCh2 );
    if ( h_Channel4->IsEnable() )   h_Channel4->AddValue( dataX, dataCh3 );
    if ( h_Channel5->IsEnable() )   h_Channel5->AddValue( dataX, dataCh4 );

    /// Incrementa contador de pacotes recebidos
    dwPktCount++;
    h_lblMsgCount->setText( QString::number( dwPktCount ) );


}

/////////////////////////////////////////////// SLIDERS /////////////////////////////////


void MainWindow::on_sliderAmplitude_valueChanged(int value)
{

    double  v1  =   0,
            v2  =   0;

    /// Convert to double
    v1  =     (double)(value / 10 );
    v2  =    (double)(value % 10 );

    dwYRange    =   ( v1 + v2/10 ) /2;

    ui->lblYRange->setText( "Amplitude: " + QString::number( dwYRange ) );

}


void MainWindow::on_sliderTimer_valueChanged(int value)
{

    ui->lblTime->setText( "Tempo: " + QString::number( value ) + " (ms)" );

    dataTimer.setInterval( value );

}



void MainWindow::on_sliderGrid_valueChanged(int value)
{
    /// Redimensionar grade
    double  v1  =   0,
            v2  =   0;

    /// Convert to double
    v1      =     (double)(value / 10 );
    v2      =    (double)(value % 10 );
    dwGrid  =   ( v1 + v2/10 );

    ui->lblGrid->setText( "Grade: " + QString::number( dwGrid ) );

}


/// Ajusta divisor do sinal de todos os canais
void MainWindow::on_sliderDivisor_valueChanged(int value)
{

    dwDivisor = value;

    if ( value == 0 )
        ui->lblDivisor->setText( "Divisor: OFF" );
    else
        ui->lblDivisor->setText( "Divisor: " + QString::number( value ) );

}


void MainWindow::on_btnOpenCloseRS_clicked()
{

    if ( ui->btnOpenCloseRS->text() == "Abrir" )
    {

        if ( h_RS->OpenPort( ui->cmbSerialList->currentText() ) )
        {

            ui->btnOpenCloseRS->setText( "Fechar" );
            h_lblSerialPort->setText( "Porta " + ui->cmbSerialList->currentText() + " aberta" );

            (void)timeLapse.restart();
            ui->customPlot->clearItems();

            h_Channel1->Clear();
            h_Channel2->Clear();
            h_Channel3->Clear();
            h_Channel4->Clear();
            h_Channel5->Clear();

            dwCaptureSecs   =   0;
            tmrCaptureCountTime.start( 1000 );

        }
        else
        {

            QMessageBox::critical(this, tr("Error"), h_RS->LastError() );
            h_lblSerialPort->setText( "Erro " + h_RS->LastError() );

        }

    }
    else
    {

        h_RS->ClosePort();

        ui->btnOpenCloseRS->setText( "Abrir" );
        h_lblSerialPort->setText( "Porta fechada" );
        tmrCaptureCountTime.stop();

    }

}

Channel *MainWindow::SelectChannel(int chId)
{
    Channel     *ch = NULL;

    switch ( chId )
    {

    case 0 :    ch = h_Channel1;        break;
    case 1 :    ch = h_Channel2;        break;
    case 2 :    ch = h_Channel3;        break;
    case 3 :    ch = h_Channel4;        break;
    case 4 :    ch = h_Channel5;        break;
    default:                            break;

    }

    return ch;
}


/////////////////////////// Controle dos canais
void MainWindow::ActivateChannel(int chId)
{


    Channel *ch = this->SelectChannel( chId );

    if ( ch != NULL )
        ch->Enable();

}

void MainWindow::DeactivateChannel(int chId)
{


    Channel *ch = this->SelectChannel( chId );

    if ( ch != NULL )
        ch->Disable();

}

void MainWindow::ChangeChannelColor(int chId, QPen pen)
{


    Channel *ch = this->SelectChannel( chId );

    if ( ch != NULL )
        ch->ChangePen( pen );

}




void MainWindow::on_btnExit_clicked()
{

    h_RS->ClosePort();
    qApp->quit();

}




void MainWindow::on_btnAbout_clicked()
{
    ///
    /// \TODO criar widget About
    ///
}




