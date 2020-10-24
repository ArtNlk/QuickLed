#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialmanager.h"
#include "screensampler.h"
#include "sunmanager.h"
#include <QThread>
#include <QColorDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    SerialManager* serialManager;
    ScreenSampler* screenSampler;
    QThread serialThread;
    QThread screenCaptureThread;
    SunManager* sunManager;
    QTimer* tickTimer;
    QColorDialog* colorpicker;

    QList<QSerialPortInfo> availablePorts;

    void getAvailablePorts();

signals:
    void serialSetBaudRate(int baudRate);
    void serialOpenPort();
    void serialOpenPortName(QString serialPortName);
    void serialClosePort();
    void serialSendData(QByteArray data);

    void startScreenCapture(captureModeFlag capMode, int numWorkers);
    void stopScreenCapture();
    void setFps(int fps);
    void singleFrame();
    //void serialRequestPorts();

private slots:
    void on_pushButton_2_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_baudRateBox_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_fpsField_textChanged(const QString &arg1);

    void on_autosampleCheckbox_stateChanged(int arg1);

    void on_testSampleButton_clicked();

    void on_wakeupTimeEditor_timeChanged(const QTime &time);

    void on_sunriseLengthEditor_valueChanged(int arg1);

    void on_TimerTick();

    void on_sleepTimeEditor_timeChanged(const QTime &time);

    void on_sunsetLengthEditor_valueChanged(int arg1);

    void on_checkBox_2_toggled(bool checked);

    void on_setColorButton_clicked();

    void on_brightnessSlider_valueChanged(int value);

public slots:
    //void recieveAvailablePorts(QList<QSerialPortInfo> ports);

private:
    Ui::MainWindow *ui;
    void setupSerial();
};
#endif // MAINWINDOW_H
