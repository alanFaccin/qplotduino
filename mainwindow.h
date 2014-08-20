#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QLabel>
#include "qcustomplot.h"
#include "channel.h"
#include "serialcommunication.h"
#include "rxchannel.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void    realtimeDataSlot();
    void    runningTime();
    void    on_sliderAmplitude_valueChanged(int value);
    void    SerialDataArrive( QString data );

    void    ActivateChannel     ( int chId );
    void    DeactivateChannel   ( int chId );
    void    ChangeChannelColor  ( int chId, QPen pen );


    void on_sliderTimer_valueChanged(int value);

    void on_sliderGrid_valueChanged(int value);

    void on_btnOpenCloseRS_clicked();

    void on_btnExit_clicked();

    void on_btnAbout_clicked();

    void on_sliderDivisor_valueChanged(int value);

private:

    Channel             *SelectChannel( int chId );
    RxChannel *AddNewChannelWidget(int chId, int x, int y);

    Ui::MainWindow      *ui;
    QTimer              dataTimer;
    QTime               timeLapse;

    Channel             *h_Channel1;
    Channel             *h_Channel2;
    Channel             *h_Channel3;
    Channel             *h_Channel4;
    Channel             *h_Channel5;


    SerialCommunication *h_RS;

    double              dwGrid;
    double              dwYRange;
    double              dataX;
    double              dataY;
    unsigned long       dwPktCount,
                        dwErrorCount;
    int                 dwDivisor;

    ///
    /// Contagem de tempo de captura de pacotes
    ///
    QTimer              tmrCaptureCountTime;
    unsigned long       dwCaptureSecs;

    QString             qsRSStatus;


    /// Status bar
    QLabel              *h_lblSerialPort;
    QLabel              *h_lblMsgCount;
    QLabel              *h_lblErrors;
    QLabel              *h_lblActiveTime;


    ///
    RxChannel           *h_WidgetChannel1;
    RxChannel           *h_WidgetChannel2;
    RxChannel           *h_WidgetChannel3;
    RxChannel           *h_WidgetChannel4;
    RxChannel           *h_WidgetChannel5;

};

#endif // MAINWINDOW_H
