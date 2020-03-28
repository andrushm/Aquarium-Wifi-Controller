#include "Arduino.h"
#include <LightOut.h>

void LightOut::init(int defaultValue)
{
    pinMode(_outPort, OUTPUT);
    digitalWrite(_outPort, defaultValue);
}

void LightOut::on()
{
    analogWrite(_outPort, lampLevel);
}
void LightOut::off()
{
    analogWrite(_outPort, LOW);
}

void LightOut::setLampBrightness(int level)
{
    lampLevel = level;
}