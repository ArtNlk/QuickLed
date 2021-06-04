#include "screensampler.h"




ScreenSampler::ScreenSampler(SerialManager* newSerialManager, int newFps)
{

    this->serialManager = newSerialManager;
    this->timer = new QTimer(this);
    this->screen = QGuiApplication::primaryScreen();

    this->screenWidth = GetSystemMetrics(SM_CXSCREEN);
    this->screenHeight = GetSystemMetrics(SM_CYSCREEN);
    this->hDesktopWnd = GetDesktopWindow();
    this->hDesktopDC = GetDC(this->hDesktopWnd);
    this->hCaptureDC = CreateCompatibleDC(this->hDesktopDC);


    qRegisterMetaType<captureModeFlag>();
    if(newFps != 0)
    {
      this->fps = newFps;
    }
    else
    {
        this->fps = 1;
    }
    connect(this,&ScreenSampler::sendData,this->serialManager,&SerialManager::send);
    connect(this->timer,&QTimer::timeout,this,&ScreenSampler::onTimer);
}

ScreenSampler::~ScreenSampler()
{
    ReleaseDC(this->hDesktopWnd, this->hDesktopDC);
    DeleteDC(this->hCaptureDC);
}

void ScreenSampler::outputAvgColor()
{
    emit requestAvgColorFromWorker(this->newWorkerId);

    if(this->newWorkerId + 1 >= this->numWorkers)
    {
        this->newWorkerId = 0;
    }
    else
    {
    this->newWorkerId++;
    }
}

void ScreenSampler::setFps(int newFps)
{
    if(newFps != 0)
    {
      this->fps = newFps;
    }
}

void ScreenSampler::start(captureModeFlag newCaptureMode, int newNumWorkers)
{
    int delay = qRound((1.0/this->fps)*1000);
    this->numWorkers = newNumWorkers;
    for(int i = 0; i<newNumWorkers; i++)
    {
        this->workerThreads.append(new QThread());
        this->workers.append(new samplerWorker(i));
        this->workers[workers.length() - 1]->moveToThread(this->workerThreads[this->workers.length() - 1]);
        connect(this->workers[workers.length() - 1],&samplerWorker::returnAvgColor,this,&ScreenSampler::getAvgColorFromWorker);
        connect(this->workerThreads[workerThreads.length() - 1], &QThread::finished, this->workerThreads[workerThreads.length() - 1], &QObject::deleteLater);
        connect(this->workerThreads[workerThreads.length() - 1], &QThread::finished, this->workers[workers.length() - 1], &QObject::deleteLater);
        connect(this,&ScreenSampler::requestAvgColorFromWorker,this->workers[workers.length() - 1],&samplerWorker::sampleAvgColor);
        this->workerThreads[workerThreads.length() - 1]->start();
    }
    this->newWorkerId = 0;
    this->captureMode = newCaptureMode;
    this->timer->setInterval(delay);
    this->timer->start();
}

void ScreenSampler::stop()
{
    this->timer->stop();
    for(int i = 0; i<this->numWorkers; i++)
    {
       this->workerThreads[i]->exit();
       while(this->workerThreads[i]->isRunning()){}
       //this->workerThreads[i]->deleteLater();
       //this->workers[i]->deleteLater();
    }
    this->workers.clear();
    this->workerThreads.clear();
}

void ScreenSampler::onTimer()
{
    switch (this->captureMode) {
    case AverageColor:
        this->outputAvgColor();
        break;

    case ScreenMapping:

        break;
    }
}

void ScreenSampler::getAvgColorFromWorker(unsigned int r, unsigned int g, unsigned int b)
{
    QByteArray message;
    message.append(3);
    message.append(3);
    message.append(r);
    message.append(g);
    message.append(b);


    emit sendData(message);
}
