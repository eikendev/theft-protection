## About

This project is a proof of concept for a do-it-yourself bag theft protection.

The concept is as follows.
Let's assume you have a bag with you where you can fit the protection hardware.
Now you want to pay less attention to your bag, while not having to worry too much.
So you go ahead and arm the device using an [NFC](https://en.wikipedia.org/wiki/Near-field_communication) tag, which acts as a "key".
Should your bag be moved, the device starts to play a loud alarm, drawing your attention back to the bag.
Using your key, you can disable the alarm and disarm it again.

Initially, I got this idea on the beach when I was worried someone might take my backpack while I'm not watching.
Be aware that this is still a prototype and that it might not be very resource efficient at the moment.

## Usage

Currently, this setup uses Arduino in combination with the [official library for the MFRC522](https://github.com/miguelbalboa/rfid).
I prepared a Makefile that instruments Arduino on the command line, so you can simply do `make install`.
Since the UID of the NFC tag is hard-coded into the program memory, you need to adjust `settings.hpp`.

## Hardware

These are the main components I used.
- Arduino Nano
- MFRC522 RFID reader
- MPU-6050 accelerometer
- alarm buzzer (12V, 110dB)
- MOSFET (to control the alarm)

I also recommend integrating a 18650 battery to make the device actually portable.
