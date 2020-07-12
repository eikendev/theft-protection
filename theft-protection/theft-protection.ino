#include <SPI.h>
#include <Wire.h>

#include "alarm.hpp"
#include "authorizer.hpp"
#include "detector.hpp"
#include "settings.hpp"

static bool is_armed = false;

static Alarm alarm;

static Authorizer authorizer;

static Detector detector;

void setup(void) {
    alarm.begin();

    Serial.begin(BAUDE_RATE);
    Serial.println(F("Starting theft protection."));

    SPI.begin();

    authorizer.begin();

    Wire.begin();

    detector.begin();
}

static void serve_alarm(void) {
    if (authorizer.is_authorized_nearby()) {
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

    detector.print_state();

    // Start the alarm if jerk indicates movement.
    if (is_armed && detector.is_triggered()) {
        alarm.enable();
    }
}

void loop(void) {
    detector.serve();

    serve_alarm();

    delay(LOOP_DELAY);
}
