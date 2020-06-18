#include <SPI.h>
#include <Wire.h>

#include <MFRC522.h>

#include "alarm.hpp"
#include "detector.hpp"
#include "settings.hpp"

static bool is_armed = false;

static const uint8_t tag01[] = {TAG01_UID};

static MFRC522 mfrc522(SS_PIN, RST_PIN);

static Alarm alarm;

static Detector detector;

void setup(void)
{
    alarm.begin();

    Serial.begin(BAUDE_RATE);
    Serial.println(F("Starting theft protection."));

    SPI.begin();

    mfrc522.PCD_Init();
    delay(4);

    Wire.begin();

    detector.begin();
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
    detector.serve();

    // Check if an authorized NFC tag is present.
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        if (is_authorized(mfrc522.uid.uidByte, mfrc522.uid.size)) {
            alarm.disable();
            delay(ALARM_TIMEOUT);

            // Unlocking gives a single buzz, locking a double buzz.
            if (is_armed) {
                alarm.buzz();
            } else {
                alarm.buzz();
                alarm.buzz();
            }

            is_armed = !is_armed;
            delay(UNLOCK_TIMEOUT);
        }
    }

    detector.print_state();

    // Start the alarm if jerk indicates movement.
    if (is_armed && detector.is_triggered()) {
        alarm.enable();
    }
}

void loop(void)
{
    serve_alarm();

    delay(LOOP_DELAY);
}
