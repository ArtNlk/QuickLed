#include "samplerworker.h"

samplerWorker::samplerWorker(int newId)
{
    this->screen = QGuiApplication::primaryScreen();

    this->id = newId;
    this->screenWidth = GetSystemMetrics(SM_CXSCREEN);
    this->screenHeight = GetSystemMetrics(SM_CYSCREEN);
    this->hDesktopWnd = GetDesktopWindow();
    this->hDesktopDC = GetDC(this->hDesktopWnd);
    this->hCaptureDC = CreateCompatibleDC(this->hDesktopDC);
}

samplerWorker::~samplerWorker()
{
    ReleaseDC(this->hDesktopWnd, this->hDesktopDC);
    DeleteDC(this->hCaptureDC);
}

void samplerWorker::sampleAvgColor(int workerId)
{
    if(workerId != this->id)
    {
        return;
    }
    HBITMAP hCaptureBitmap = CreateCompatibleBitmap(this->hDesktopDC, this->screenWidth, this->screenHeight);
    SelectObject(hCaptureDC, hCaptureBitmap);

    BitBlt(this->hCaptureDC, 0, 0, this->screenWidth, this->screenHeight, this->hDesktopDC, 0,0, SRCCOPY|CAPTUREBLT);

    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = this->screenWidth;
    bmi.bmiHeader.biHeight = this->screenHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    int size = this->screenWidth * this->screenHeight * 4;
    unsigned char *pPixels = new unsigned char[size];

    GetDIBits(this->hCaptureDC, hCaptureBitmap, 0, this->screenHeight, pPixels, &bmi,    DIB_RGB_COLORS);

    unsigned int avgR = 0;
    unsigned int avgG = 0;
    unsigned int avgB = 0;
    size = size /4;
    for (int i = 0; i < size; i++) {
        avgB = avgB + pPixels[4*i + 0];
        avgG = avgG + pPixels[4*i + 1];
        avgR = avgR + pPixels[4*i + 2];
    }

    avgR = avgR / (size);
    avgG = avgG / (size);
    avgB = avgB / (size);

    emit returnAvgColor(avgR,avgG,avgB);

    delete[] pPixels;

    DeleteObject(hCaptureBitmap);
}
