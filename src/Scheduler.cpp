
#include "Arduino.h"
#include <Scheduler.h>
#include <time.h>

// Constructor
Scheduler::Scheduler(DeviceOut deviceOut)
{
    _deviceOut = deviceOut;
};
void Scheduler::tick()
{
    now = time(nullptr);
    // int nowHour = hour(now);
    // if ((now - lastTime) > 60) {
    int offset = now % 3600;
    _hour = now / 3600;
    _mins = offset / 60;
    if (_lastTime != _mins)
    {
        _lastTime = _mins;
        checkTime();
        _minute_tick_callback();
    };
};

void Scheduler::checkTime() {
    boolean isCheckOk = (isTimeOk() || isMinutesOk());
    if (isCheckOk) {
        _deviceOut.on();
    } else {
        _deviceOut.off();
    }
    _check_time_callback(isCheckOk);
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
    _timeOn = setTimeOn;
};
void Scheduler::setTimeOff(TIME setTimeOff){
    _timeOff = setTimeOff;
};

int Scheduler::Test()
{
    return 21;
};

void Scheduler::onMinuteTick(THandlerFunction fn) {
    _minute_tick_callback = fn;
}

void Scheduler::onCheckTimeCallback(THandlerBooleanFunction fn) {
    _check_time_callback = fn;
}

// Scheduler& Scheduler::setCallback(MY_CALLBACK_SIGNATURE) {
//     this->callback = callback;
//     return *this;
// };
