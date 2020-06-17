# SEE https://github.com/arduino/Arduino/blob/master/build/shared/manpage.adoc

.PHONY: build
build:
	arduino --verify --board arduino:avr:nano:cpu=atmega328old --verbose theft-protection/theft-protection.ino

.PHONY: install
install:
	arduino --upload --board arduino:avr:nano:cpu=atmega328old --port /dev/ttyUSB0 --verbose theft-protection/theft-protection.ino

.PHONY: monitor
monitor:
	minicom -b 9600 -D /dev/ttyUSB0
