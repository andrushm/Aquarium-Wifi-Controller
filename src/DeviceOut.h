#ifndef Scheduler_h
#define Scheduler_h

#include <Arduino.h>

#endif

class DeviceOut {
    public:
    DeviceOut(int portName);
    virtual void init(int defaultValue = LOW);
    virtual void on();
    virtual void off();
    protected:
    int _outPort;
};