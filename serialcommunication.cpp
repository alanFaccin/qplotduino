
#include "serialcommunication.h"

#include <QtSerialPort/QSerialPortInfo>



SerialCommunication::SerialCommunication( QObject *parent )
{

    serial  =   new QSerialPort( parent );
    connect( serial, SIGNAL(readyRead()), this, SLOT(readReady()) );

    rxData.clear();

}


SerialCommunication::~SerialCommunication()
{

    if ( serial->isOpen() )
        serial->close();

    delete serial;

    serial = NULL;

}

void SerialCommunication::PopulateSerialList()
{

    serialList.clear();

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {

        serialList.append( info.portName() );

    }

}

QStringList SerialCommunication::getSerialList()
{
    return serialList;
}

QString SerialCommunication::LastError()
{
    return serial->errorString();
}

void SerialCommunication::readReady()
{

    /// data pronta para ser lida
///    rxData


    char    data;

    while (serial->isOpen() &&  serial->read( &data, 1 ) )
    {

        if ( data == '\n' )
        {

            emit DataArrive( rxData );

            rxData.clear();

        }
        else if ( data != '\r' )
            rxData += data;

    }

}

bool SerialCommunication::OpenPort(QString portName)
{

    serial->setPortName( portName );
    serial->setBaudRate( QSerialPort::Baud9600 );
    serial->setDataBits( QSerialPort::Data8 );
    serial->setParity( QSerialPort::NoParity );
    serial->setStopBits( QSerialPort::OneStop );
    serial->setFlowControl( QSerialPort::NoFlowControl );
    serial->setDataTerminalReady( false );
    serial->setSettingsRestoredOnClose( false );

    ///
    ///
    ///
    if ( !serial->open(QIODevice::ReadOnly) )
        return false;

    serial->clear();

    return true;

}

void SerialCommunication::ClosePort()
{

    if ( serial->isOpen() )
        serial->close();

}

