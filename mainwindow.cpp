#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QComboBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupSerial();
    this->screenSampler = new ScreenSampler(this->serialManager, 10);
    this->screenSampler->moveToThread(&this->screenCaptureThread);
    this->sunManager = new SunManager(QTime::currentTime(),QTime::currentTime(),60,60);
    this->tickTimer = new QTimer();
    this->tickTimer->setInterval(100);
    this->colorpicker = new QColorDialog();
    connect(this,&MainWindow::startScreenCapture,this->screenSampler,&ScreenSampler::start);
    connect(this,&MainWindow::stopScreenCapture,this->screenSampler,&ScreenSampler::stop);
    connect(this,&MainWindow::setFps,this->screenSampler,&ScreenSampler::setFps);
    connect(this->tickTimer,&QTimer::timeout,this,&MainWindow::on_TimerTick);
    //connect(this,&MainWindow::singleFrame,this->screenSampler,&ScreenSampler::outputAvgColor);
    this->screenCaptureThread.start();
}

MainWindow::~MainWindow()
{
    delete ui;
    this->serialManager->closePort();
    this->serialThread.exit();
    this->screenCaptureThread.exit();
}

void MainWindow::setupSerial()
{
    this->serialManager = new SerialManager();
    this->serialManager->moveToThread(&this->serialThread);
    connect(this,&MainWindow::serialOpenPortName,this->serialManager,&SerialManager::openPortName);
    connect(this,&MainWindow::serialOpenPort,this->serialManager,&SerialManager::openPort);
    connect(this,&MainWindow::serialClosePort,this->serialManager,&SerialManager::closePort);
    connect(this,&MainWindow::serialSetBaudRate,this->serialManager,&SerialManager::setBaudRate);
    connect(this,&MainWindow::serialSendData,this->serialManager,&SerialManager::send);
    //connect(this,&MainWindow::serialRequestPorts,serialManager,&SerialManager::getAvailablePorts);
    //connect(serialManager,&SerialManager::sendAvailablePorts,this,&MainWindow::recieveAvailablePorts);
    this->serialThread.start();
    this->getAvailablePorts();
}

void MainWindow::on_pushButton_2_clicked()
{
    this->getAvailablePorts();
}

void MainWindow::getAvailablePorts()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    ui->portListBox->clear();
    for(int i = 0; i < ports.length(); i++)
    {
        ui->portListBox->addItem(ports.at(i).portName());
    }
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    ui->portNameLabel->setText(ui->portListBox->currentText());


    if(arg1 == 2)
    {
        emit serialOpenPortName(ui->portListBox->currentText());
    }
    else if (arg1 == 0)
    {
        emit serialClosePort();
    }
}

void MainWindow::on_baudRateBox_currentTextChanged(const QString &arg1)
{
    ui->baudRateLabel->setText(ui->baudRateBox->currentText());
    emit serialSetBaudRate(ui->baudRateBox->currentText().toInt());
}

void MainWindow::on_pushButton_clicked()
{
    QByteArray message;
    QList<QString> byteStrings = ui->serialInput->text().split(" ",Qt::SplitBehaviorFlags::SkipEmptyParts);
    for(int i = 0; i < byteStrings.length(); i++)
    {
        message.append((char)byteStrings.at(i).toUInt());
    }
    emit serialSendData(message);
}

void MainWindow::on_fpsField_textChanged(const QString &arg1)
{
    emit setFps(arg1.toInt());
}

void MainWindow::on_autosampleCheckbox_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        emit startScreenCapture(captureModeFlag::AverageColor,this->ui->numWorkersSpinBox->value());
    }
    else if (arg1 == 0)
    {
        emit stopScreenCapture();
    }
}

void MainWindow::on_testSampleButton_clicked()
{
    emit singleFrame();
}



void MainWindow::on_wakeupTimeEditor_timeChanged(const QTime &time)
{
    this->sunManager->setTargetSunriseTime(time);
    ui->sleepTimeEditor->setMinimumTime(time.addSecs(ui->sunsetLengthEditor->value()*60));
}

void MainWindow::on_sunriseLengthEditor_valueChanged(int arg1)
{
    this->sunManager->setSunriseLength(arg1);
}

void MainWindow::on_sleepTimeEditor_timeChanged(const QTime &time)
{
    this->sunManager->setTargetSunsetTime(time);
}

void MainWindow::on_sunsetLengthEditor_valueChanged(int arg1)
{
    this->sunManager->setSunsetLength(arg1);
    ui->sleepTimeEditor->setMinimumTime(ui->wakeupTimeEditor->time().addSecs(ui->sunsetLengthEditor->value()*60));
}

void MainWindow::on_TimerTick()
{
    QColor color = this->sunManager->requestCurrentColor();
    QByteArray data;
    data.append(3);
    data.append(3);
    data.append((char)color.red());
    data.append((char)color.green());
    data.append((char)color.blue());

    emit this->serialSendData(data);
}

void MainWindow::on_checkBox_2_toggled(bool checked)
{
    if (checked)
    {
        this->tickTimer->start();
    }
    else
    {
        this->tickTimer->stop();
    }
}

void MainWindow::on_setColorButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this);

    if(color.isValid())
    {
        QByteArray data;
        data.append(3);
        data.append(3);
        data.append((char)color.red());
        data.append((char)color.green());
        data.append((char)color.blue());

        emit this->serialSendData(data);
    }
}

void MainWindow::on_brightnessSlider_valueChanged(int value)
{
    QByteArray data;
    data.append(2);
    data.append(1);
    data.append((char)roundf((value/100.0)*255.0));

    emit this->serialSendData(data);
}
