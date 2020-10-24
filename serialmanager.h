#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QObject>

//struct SerialSettings
//{
//    QString portName = "";
//    QSerialPort::BaudRate baudRate = QSerialPort::Baud115200;
//    QSerialPort::DataBits dataBits = QSerialPort::Data8;
//    QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl;
//    QSerialPort::Parity parity = QSerialPort::NoParity;
//    QSerialPort::StopBits stopBits = QSerialPort::OneStop;
//    int readTimeoutMs = 10;
//    int timeoutMs = 100;
//};
class SerialManager : public QObject
{
    Q_OBJECT

public:
    SerialManager();
    SerialManager(QSerialPortInfo settings);
    ~SerialManager();



    QList<QSerialPortInfo> availablePorts;

    QSerialPortInfo portSettings;
    QSerialPort* serialPort;
    bool isOpen = false;

signals:
    void dataRecieved(QByteArray data);
    void error(QSerialPort::SerialPortError error);
    //void sendAvailablePorts(QList<QSerialPortInfo> ports);

private:
    bool ready;



public slots:
    void serialError(QSerialPort::SerialPortError serialError);
    void serialReadyRead();

    void openPortName(QString newPortName);
    void openPort();
    void closePort();
    void send(QByteArray data);
    void updateSettings(QSerialPortInfo newSettings);
    void setBaudRate(int newBaudRate);
    //void getAvailablePorts();

};



#endif // SERIALMANAGER_H
