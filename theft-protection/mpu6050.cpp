// Based on work of arduino.cc user "Krodal".
// https://playground.arduino.cc/Main/MPU-6050/

#include "mpu6050.hpp"

#include <Wire.h>

#define SWAP(x, y) \
    swap = x;      \
    x = y;         \
    y = swap

/* Read multiple bytes from an I2C device. It uses the boolean parameter for
 * Wire.endTransMission() to be able to hold or release the I2C-bus. */
int mpu6050_read(int dst_addr, uint8_t* buffer, int size) {
    int i, n, error;

    Wire.beginTransmission(MPU6050_I2C_ADDRESS);
    n = Wire.write(dst_addr);
    if (n != 1) return (-10);

    n = Wire.endTransmission(false);
    if (n != 0) return (n);

    // Third parameter is true: release I2C-bus after data is read.
    Wire.requestFrom(MPU6050_I2C_ADDRESS, size, true);
    i = 0;
    while (Wire.available() && i < size) {
        buffer[i++] = Wire.read();
    }
    if (i != size) return (-11);

    return 0;
}

/* Write multiple bytes to an I2C device. */
int mpu6050_write(int dst_addr, const uint8_t* src_addr, int size) {
    int n, error;

    Wire.beginTransmission(MPU6050_I2C_ADDRESS);
    n = Wire.write(dst_addr);
    if (n != 1) return (-20);

    n = Wire.write(src_addr, size);
    if (n != size) return (-21);

    error = Wire.endTransmission(true);
    if (error != 0) return (error);

    return 0;
}

/* Write a single register. */
int mpu6050_write_reg(int reg, uint8_t data) {
    int error;

    error = mpu6050_write(reg, &data, 1);

    return error;
}

void mpu6050_swap_registers(union accel_t_gyro_union* data) {
    uint8_t swap;

    SWAP(data->reg.x_accel_h, data->reg.x_accel_l);
    SWAP(data->reg.y_accel_h, data->reg.y_accel_l);
    SWAP(data->reg.z_accel_h, data->reg.z_accel_l);
    SWAP(data->reg.t_h, data->reg.t_l);
    SWAP(data->reg.x_gyro_h, data->reg.x_gyro_l);
    SWAP(data->reg.y_gyro_h, data->reg.y_gyro_l);
    SWAP(data->reg.z_gyro_h, data->reg.z_gyro_l);
}
