#ifndef __ALARM_HPP__
#define __ALARM_HPP__

#include <Arduino.h>

class Alarm {
    private:

    public:
        Alarm(void);

        void begin(void);

        void enable(void);

        void disable(void);

        void buzz(void);
};

#endif
