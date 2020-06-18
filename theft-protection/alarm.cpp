#include "alarm.hpp"
#include "settings.hpp"

Alarm::Alarm(void)
{
}

void Alarm::begin(void)
{
    pinMode(ALARM_PIN, OUTPUT);
    this->disable();
}

void Alarm::enable(void)
{
    digitalWrite(ALARM_PIN, HIGH);
}

void Alarm::disable(void)
{
    digitalWrite(ALARM_PIN, LOW);
}

void Alarm::buzz(void)
{
    this->enable();
    delay(ALARM_TIMEOUT);
    this->disable();
    delay(ALARM_TIMEOUT);
}
