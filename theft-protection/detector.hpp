#ifndef __DETECTOR_HPP__
#define __DETECTOR_HPP__

#include <Arduino.h>

#include "mpu6050.hpp"

class Detector {
    private:
        union accel_t_gyro_union previous;

        uint32_t diff_x = 0;
        uint32_t diff_y = 0;
        uint32_t diff_z = 0;

    public:
        Detector(void);

        void begin(void);

        void serve(void);

        void print_state(void);

        bool is_triggered(void);
};

#endif
