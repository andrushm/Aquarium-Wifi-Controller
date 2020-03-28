#include <DeviceOut.h>
#ifndef LightOut_h
#define LightOut_h

#include <Arduino.h>

#endif

class LightOut : public DeviceOut
{
private:
    int lampLevel = 0;
public:
    void init(int defaultValue = 0);
    void on();
    void off();
    void setLampBrightness(int level);
    // PumpOut(/* args */);
    // ~PumpOut();
};

// PumpOut::PumpOut(/* args */)
// {
// }

// PumpOut::~PumpOut()
// {
// }
