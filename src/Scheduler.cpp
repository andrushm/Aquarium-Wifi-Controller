
#include "Arduino.h"
#include <Scheduler.h>

// Constructor
Scheduler::Scheduler(int portName)
{
    _outPort = portName;
    pinMode(_outPort, OUTPUT);
    digitalWrite(_outPort, LOW);
};
void Scheduler::tick()
{
    long now = time(nullptr);
    // int nowHour = hour(now);
    // if ((now - lastTime) > 60) {
    int offset = now % 3600;
    _hour = now / 3600;
    _mins = offset / 60;
    if (_lastTime != _mins)
    {
        _lastTime = _mins;
        checkTime();
    };
};

void Scheduler::checkTime() {
    if (isTimeOk() || isMinutesOk()) {
        digitalWrite(_outPort, HIGH);
    } else {
        digitalWrite(_outPort, LOW);
    }

    // @TODO Added callback function.
}

boolean Scheduler::isTimeOk() {

    if (_timeOn.hour > _timeOff.hour) {
        return (((_timeOn.hour < _hour) && (_timeOff.hour > _hour)) || isMinutesOk());
            // && ((_timeOn.mins >= _mins) && (_timeOff.mins < _mins))
    }

    return ((_timeOn.hour > _hour) && (_timeOff.hour < _hour)) || isMinutesOk();
};

boolean Scheduler::isMinutesOk()
{
    return ((_timeOn.hour == _hour) && (_timeOn.mins >= _mins)) || ((_timeOff.hour == _hour) && (_timeOff.mins > _mins));
};
// void Scheduler::setOutPort(int portName){
// };
void Scheduler::setTimeOn(TIME setTimeOn){

};
void Scheduler::setTimeOff(TIME setTimeOff){

};

int Scheduler::Test()
{
    return 21;
};

Scheduler& Scheduler::setCallback(MY_CALLBACK_SIGNATURE) {
    this->callback = callback;
    return *this;
};
