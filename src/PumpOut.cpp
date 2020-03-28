#include "Arduino.h"
#include <PumpOut.h>

void PumpOut::init(int defaultValue)
{
    pinMode(_outPort, OUTPUT);
    digitalWrite(_outPort, defaultValue);
}

void PumpOut::on()
{
    digitalWrite(_outPort, HIGH);
}
void PumpOut::off()
{
    digitalWrite(_outPort, LOW);
}