#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H


#include <QSerialPort>
#include <QStringList>
#include <QString>
#include <QMutex>


class SerialCommunication : public QObject
{

    Q_OBJECT

public:
    SerialCommunication(QObject *parent = 0);
    ~SerialCommunication();


    void            PopulateSerialList          ( );

    bool            OpenPort                    ( QString portName );
    void            ClosePort                   ( );

    QStringList     getSerialList               ( );
    QString         LastError                   ( );

signals:
    void            DataArrive                  ( QString data );

private slots:
    void            readReady                   ( );

private:
    QSerialPort     *serial;
    QStringList     serialList;

    QString         rxData;

};

#endif // SERIALCOMMUNICATION_H
