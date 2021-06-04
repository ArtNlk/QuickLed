#include <sunmanager.h>

SunManager::SunManager(QTime targetSunriseTime_,QTime targetSunsetTime_, int sunriseLengthMins_,int sunsetLengthMins_) : QObject()
{
    this->maxTemp = 6000;
    this->minTemp = 800;

    this->sunriseLength = sunriseLengthMins_*60*1000;
    this->targetSunriseTime = targetSunriseTime_;

    this->sunsetLength = sunsetLengthMins_*60*1000;
    this->targetSunsetTime = targetSunsetTime_;
}

void SunManager::setTargetSunriseTime(QTime targetSunriseTime_)
{
    this->targetSunriseTime = targetSunriseTime_;
}

void SunManager::setTargetSunriseTime(int hr, int min)
{
    this->targetSunriseTime.setHMS(hr,min,0);
}

void SunManager::setTargetSunsetTime(QTime targetSunsetTime_)
{
    this->targetSunsetTime = targetSunsetTime_;
}

void SunManager::setTargetSunsetTime(int hr, int min)
{
    this->targetSunsetTime.setHMS(hr,min,0);
}

void SunManager::setSunsetLength(int lengthMins)
{
    this->sunsetLength = lengthMins*60*1000;
}

void SunManager::setSunriseLength(int lengthMins)
{
    this->sunriseLength = lengthMins*60*1000;
}

QColor SunManager::requestCurrentColor()
{
    QTime currentTime = QTime::currentTime();
    qint64 secondsUntilSunrise = currentTime.msecsTo(this->targetSunriseTime);
    qint64 secondsUntilSunset = currentTime.msecsTo(this->targetSunsetTime);

    int newTemperature = 0;
    if(secondsUntilSunrise <= 0 and secondsUntilSunset <= 0)//after sunset
    {
        return QColor(0,0,0);
    }
    else if(secondsUntilSunset > 0 and secondsUntilSunrise <=0)//before sunset end time
    {
        if(secondsUntilSunset < this->sunsetLength)//in sunset
        {
            newTemperature = (float)this->maxTemp*((float)secondsUntilSunset / (float)this->sunsetLength);
        }
        else
        {
            return QColor(tempToRed(this->maxTemp),tempToGreen(this->maxTemp),tempToBlue(this->maxTemp));
        }
    }
    else if(secondsUntilSunset > 0 and secondsUntilSunrise > 0) //before sunrise end time
    {
        if(secondsUntilSunrise < this->sunriseLength)//in sunrise
        {
            newTemperature = ((float)this->maxTemp - (float)this->maxTemp*((float)secondsUntilSunrise / (float)this->sunriseLength));
        }
        else
        {
            return QColor(0,0,0);
        }
    }

    return this->colorFromTemp(newTemperature+800);
}

QColor SunManager::colorFromTemp(int temperature)
{
    float brightness = (float)(temperature - 800) / 6000;
    brightness = tanh(brightness*5 - 2)/2 + 0.5;
    if (brightness > 1)
    {
        brightness = 1;
    } else if (brightness < 0)
    {
        brightness = 0;
    }
    //qDebug("t: %d; b: brightness %f",temperature,brightness);
    int red = (int)roundf(tempToRed(temperature) * brightness);
    int green = (int)roundf(tempToGreen(temperature) * brightness);
    int blue = (int)roundf(tempToBlue(temperature) * brightness);
    return QColor(red,green,blue);
}

int SunManager::tempToRed(int temperature)
{
    float red = 255;
    float newTemperature = (float)temperature/100.0;
    if(newTemperature>66) {
        red = newTemperature - 60.0;
        red = 329.698727466 * pow(red,-0.1332047592);
    }

    if(red > 255)
    {
        return 255;
    }

    if(red < 0)
    {
        return 0;
    }

    return red;
}

int SunManager::tempToGreen(int temperature)
{
    float green;
    float newTemperature = (float)temperature/100.0;
    if(newTemperature<= 66){
        green = newTemperature;
        green = (99.4708025861 * log(green)) - 161.1195681661;
    } else {
        green = newTemperature - 60.0;
        green = 288.1221695283 * pow(green, -0.0755148492);
    }
    if(green > 255)
    {
        return 255;
    }

    if(green < 0)
    {
        return 0;
    }

    return green;
}

int SunManager::tempToBlue(int temperature)
{
    float blue = 255;
    float newTemperature = (float)temperature/100.0;
    if(newTemperature<65)
    {
        if(newTemperature <= 19)
        {
          blue = 0;
        }
        else
        {
          blue = newTemperature - 10.0;
          blue = (138.5177312231 * log(blue)) - 305.0447927307;
        }
    }

    if(blue > 255)
    {
        return 255;
    }

    if(blue < 0)
    {
        return 0;
    }

    return blue;
}
