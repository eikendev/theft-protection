#include "detector.hpp"

#include "settings.hpp"

#define ACCEL_DIFF(a, b) (abs(a - b))
#define ACCEL_TRIGGERS(a) (a > ACCEL_MAX_DELTA)

Detector::Detector(void) {}

void Detector::begin(void) {
    // Clear the 'sleep' bit to start the sensor.
    mpu6050_write_reg(MPU6050_PWR_MGMT_1, 0);
}

void Detector::serve(void) {
    int error;
    union accel_t_gyro_union current;

    // Read new values from the accelerometer.
    error = mpu6050_read(MPU6050_ACCEL_XOUT_H, (uint8_t*)&current, sizeof(current));
    if (error != 0) {
        Serial.print(F("Error while reading: "));
        Serial.println(error, DEC);
        return;
    }

    // Swap the high and low registers for the new values.
    mpu6050_swap_registers(&current);

    this->diff_x = ACCEL_DIFF(current.value.x_accel, this->previous.value.x_accel);
    this->diff_y = ACCEL_DIFF(current.value.y_accel, this->previous.value.y_accel);
    this->diff_z = ACCEL_DIFF(current.value.z_accel, this->previous.value.z_accel);

    memcpy(&this->previous, &current, sizeof(union accel_t_gyro_union));
}

void Detector::print_state(void) {
    Serial.println(F("Difference in acceleration is:"));
    Serial.println(this->diff_x);
    Serial.println(this->diff_y);
    Serial.println(this->diff_z);
}

bool Detector::is_triggered(void) {
    const bool has_changed =
        (ACCEL_TRIGGERS(this->diff_x) || ACCEL_TRIGGERS(this->diff_y) || ACCEL_TRIGGERS(this->diff_z));

    return has_changed;
}
