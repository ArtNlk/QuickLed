#include "serialmanager.h"
#include <QSerialPort>
#include <QSerialPortInfo>

SerialManager::SerialManager() : QObject()
{
    this->serialPort = new QSerialPort(this);
    connect(this->serialPort,&QSerialPort::errorOccurred,this,&SerialManager::serialError);
    connect(this->serialPort, &QSerialPort::readyRead,this, &SerialManager::serialReadyRead);
}

SerialManager::SerialManager(QSerialPortInfo settings) : QObject()
{
    connect(this->serialPort,&QSerialPort::errorOccurred,this,&SerialManager::serialError);
    connect(this->serialPort, &QSerialPort::readyRead,this, &SerialManager::serialReadyRead);
    this->portSettings = settings;
}

SerialManager::~SerialManager()
{
    this->closePort();
}

void SerialManager::openPort()
{
    this->serialPort->setPort(this->portSettings);
    bool success = this->serialPort->open(QIODevice::ReadWrite);
    if(success)
    {
        this->isOpen = true;
    }
    else
    {
        emit this->error(QSerialPort::UnknownError);
    }
}

void SerialManager::openPortName(QString newPortName)
{
    portSettings = QSerialPortInfo(newPortName);
    this->serialPort->setPort(portSettings);
    bool success = this->serialPort->open(QIODevice::ReadWrite);
    if(success)
    {
        this->isOpen = true;
    }
    else
    { 
        emit error(QSerialPort::UnknownError);
        qWarning("Port open error");
    }
}

void SerialManager::closePort()
{
    this->serialPort->close();
}

void SerialManager::send(QByteArray data)
{
    if(this->ready)
    {
        if(this->serialPort->isOpen())
        {
            this->serialPort->write(data);
            this->ready = false;
        }
        else
        {
            emit error(QSerialPort::NotOpenError);
        }
    }
}

void SerialManager::updateSettings(QSerialPortInfo newSettings)
{
    this->portSettings = newSettings;
}

void SerialManager::setBaudRate(int newBaudRate)
{
    this->serialPort->setBaudRate(newBaudRate);
}

//void SerialManager::getAvailablePorts()
//{
//   this->availablePorts = QSerialPortInfo::availablePorts();
//    emit sendAvailablePorts(this->availablePorts);
//}

void SerialManager::serialError(QSerialPort::SerialPortError serialError)
{
    emit error(serialError);
    qWarning("Serial port error: %u", serialError);
}

void SerialManager::serialReadyRead()
{
    QByteArray serialData = this->serialPort->readAll();
    if(!this->ready and serialData.at(0) == 1)
    {
        this->ready = true;
    }
    else
    {
        emit dataRecieved(serialData);
    }
}

