#ifndef Scheduler_h
#define Scheduler_h

#include <Arduino.h>

// unuxTime / 3600 = hour;
// 'y' => secs / 31556926 % 12,
// 'w' => secs / 604800 % 52,
// 'd' => secs / 86400 % 7,
// 'h' => secs / 3600 % 24,
// 'm' => secs / 60 % 60,
// 's' => secs % 60

// hours = offset / 3600;
// offset = offset % 3600;
// mins = offset / 60;
// secs = offset % 60;

#if defined(ESP8266) || defined(ESP32)
#include <functional>
#define MY_CALLBACK_SIGNATURE std::function<void(char*, uint8_t*, unsigned int)> callback
#else
#define MY_CALLBACK_SIGNATURE void (*callback)(char*, uint8_t*, unsigned int)
#endif

struct TIME {
    int hour;
    int mins;
};

class Scheduler
{
private:
    int i = 0;
    int _outPort;
    long _lastTime = 0;
    int _hour = 0;
    int _mins = 0;
    MQTT_CALLBACK_SIGNATURE;

    TIME _timeOn = {0, 0};
    TIME _timeOff = {0, 0};

    void checkTime();
    boolean isTimeOk();
    boolean isMinutesOk();

public:
    Scheduler(int portName);
    void tick();
    // void setOutPort();
    void setTimeOn(TIME setTimeOn);
    void setTimeOff(TIME setTimeOff);
    // void setOn
    int Test();
    Scheduler& setCallback(MY_CALLBACK_SIGNATURE);
};

#endif