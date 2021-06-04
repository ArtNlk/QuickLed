#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <QObject>
#include <serialmanager.h>
#include <QTime>
#include <QDateTime>
#include <math.h>
#include<QDebug>
#include <QColor>

class SunManager : public QObject
{
    Q_OBJECT
public:
    SunManager(QTime targetSunriseTime_,QTime targetSunsetTime,int sunriseLengthMins_, int sunsetLengthMins_);
    void setSunriseLength(int length);
    void setSunsetLength(int length);
    void setTargetSunriseTime(QTime targetTime_);
    void setTargetSunriseTime(int hr, int min);
    void setTargetSunsetTime(QTime targetTime_);
    void setTargetSunsetTime(int hr, int min);
    QColor colorFromTemp(int temp);
    QColor requestCurrentColor();
private:
    QColor tempToRgb(int temperature);
    int tempToRed(int temperature);
    int tempToGreen(int temperature);
    int tempToBlue(int temperature);
    QTime targetSunriseTime;
    QTime targetSunsetTime;
    int sunriseLength;
    int sunsetLength;

    int maxTemp;
    int minTemp;
};

#endif // TIMEMANAGER_H
