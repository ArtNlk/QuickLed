#ifndef SAMPLERWORKER_H
#define SAMPLERWORKER_H

#include <QObject>
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

class samplerWorker : public QObject
{
    Q_OBJECT
public:
    samplerWorker(int newId);
    ~samplerWorker();
    QScreen* screen;
    QImage screenshot;
    int fps;
    int id;
    int screenWidth;
    int screenHeight;
    HWND hDesktopWnd;
    HDC hDesktopDC;
    HDC hCaptureDC;

signals:
    void returnAvgColor(unsigned int r, unsigned int g, unsigned int b);

public slots:
    void sampleAvgColor(int workerId);
};

#endif // SAMPLERWORKER_H
