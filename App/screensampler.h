#ifndef SCREENSAMPLER_H
#define SCREENSAMPLER_H

#include <QObject>
#include <QPixmap>
#include <QTimer>
#include <QScreen>
#include <QApplication>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QWindow>
#include <QFile>
#include <QImage>
#include <QPixmap>
#include <QRgb>
#include <QtOpenGL>
#include <windows.h>
#include <wingdi.h>
#include <serialmanager.h>
#include <samplerworker.h>

enum captureModeFlag
{
    AverageColor,
    ScreenMapping
};

class ScreenSampler : public QObject
{
    Q_OBJECT
public:
    ScreenSampler(SerialManager* newSerialManager, int newFps);
    ~ScreenSampler();


    QTimer* timer;
    SerialManager* serialManager;
    QScreen* screen;
    QImage screenshot;

    QList<QThread*> workerThreads;
    QList<samplerWorker*> workers;
    int fps;
    int numWorkers;
    int newWorkerId;
    int screenWidth;
    int screenHeight;
    HWND hDesktopWnd;
    HDC hDesktopDC;
    HDC hCaptureDC;
    captureModeFlag captureMode;




signals:
    void sendAvgColor();
    void requestAvgColorFromWorker(int workerId);
    void sendData(QByteArray data);

public slots:
    void outputAvgColor();
    void getAvgColorFromWorker(unsigned int r, unsigned int g, unsigned int b);
    void setFps(int fps);
    void start(captureModeFlag captureMode, int newNumWorkers);
    void stop();

private slots:
    void onTimer();
};
    Q_DECLARE_METATYPE(captureModeFlag);

#endif // SCREENSAMPLER_H
