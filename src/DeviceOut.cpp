#include "Arduino.h"
#include <DeviceOut.h>

DeviceOut::DeviceOut(int portName)
{
    _outPort = portName;
    init();
};

