#include <SPI.h>
#include <Wire.h>

#include <MFRC522.h>

#include "settings.hpp"
#include "mpu6050.hpp"

#define ACCEL_DIFF(a, b) (abs(a - b))
#define ACCEL_TRIGGERS(a, b) (ACCEL_DIFF(a, b) > ACCEL_MAX_DELTA)

#define ALARM_DISABLE digitalWrite(ALARM_PIN, LOW)
#define ALARM_ENABLE digitalWrite(ALARM_PIN, HIGH)

MFRC522 mfrc522(SS_PIN, RST_PIN);

static const uint8_t tag01[] = {TAG01_UID};

static bool is_armed = false;

static union accel_t_gyro_union data_old;

void setup(void)
{
    pinMode(ALARM_PIN, OUTPUT);
    ALARM_DISABLE;

    Serial.begin(BAUDE_RATE);
    Serial.println(F("Starting theft protection."));

    SPI.begin();

    mfrc522.PCD_Init();
    delay(4);

    Wire.begin();

    // Clear the 'sleep' bit to start the sensor.
    mpu6050_write_reg(MPU6050_PWR_MGMT_1, 0);
}

static void print_accel_diff(
    accel_t_gyro_union *current,
    accel_t_gyro_union *old
) {
    Serial.println(F("Difference in acceleration is:"));
    Serial.println(ACCEL_DIFF(current->value.x_accel, old->value.x_accel));
    Serial.println(ACCEL_DIFF(current->value.y_accel, old->value.y_accel));
    Serial.println(ACCEL_DIFF(current->value.z_accel, old->value.z_accel));
}

static bool do_trigger_alarm(
    accel_t_gyro_union *current,
    accel_t_gyro_union *old
) {
    if (ACCEL_TRIGGERS(current->value.x_accel, old->value.x_accel) ||
        ACCEL_TRIGGERS(current->value.y_accel, old->value.y_accel) ||
        ACCEL_TRIGGERS(current->value.z_accel, old->value.z_accel)) {
        return true;
    }

    return false;
}

static void trigger_buzz(void)
{
    ALARM_ENABLE;
    delay(ALARM_TIMEOUT);
    ALARM_DISABLE;
    delay(ALARM_TIMEOUT);
}

static bool is_authorized(
    byte *buffer,
    byte size
) {
    if (size == sizeof(tag01) && memcmp(buffer, tag01, size) == 0) {
        return true;
    }

    return false;
}

static void serve_alarm(void)
{
    int error;
    union accel_t_gyro_union data_new;

    // Read new values from the accelerometer.
    error = mpu6050_read(
        MPU6050_ACCEL_XOUT_H,
        (uint8_t *) &data_new,
        sizeof(data_new)
    );
    if (error != 0) {
        Serial.print(F("Error while reading: "));
        Serial.println(error, DEC);
        return;
    }

    // Swap the high and low registers for the new values.
    mpu6050_swap_registers(&data_new);

    // Check if an authorized NFC tag is present.
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        if (is_authorized(mfrc522.uid.uidByte, mfrc522.uid.size)) {
            ALARM_DISABLE;
            delay(ALARM_TIMEOUT);

            // Unlocking gives a single buzz, locking a double buzz.
            if (is_armed) {
                trigger_buzz();
            } else {
                trigger_buzz();
                trigger_buzz();
            }

            is_armed = !is_armed;
            delay(UNLOCK_TIMEOUT);
        }
    }

    print_accel_diff(&data_new, &data_old);

    // Start the alarm if jerk indicates movement.
    if (is_armed && do_trigger_alarm(&data_new, &data_old)) {
        ALARM_ENABLE;
    }

    memcpy(&data_old, &data_new, sizeof(data_old));
}

void loop(void)
{
    serve_alarm();

    delay(LOOP_DELAY);
}
