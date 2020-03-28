#include <DeviceOut.h>
#ifndef PumpOut_h
#define PumpOut_h

#include <Arduino.h>

#endif

class PumpOut : public DeviceOut
{
private:
    /* data */
public:
    void init(int defaultValue = LOW);
    void on();
    void off();
    // PumpOut(/* args */);
    // ~PumpOut();
};

// PumpOut::PumpOut(/* args */)
// {
// }

// PumpOut::~PumpOut()
// {
// }
