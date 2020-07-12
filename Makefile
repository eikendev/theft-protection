# SEE https://github.com/arduino/Arduino/blob/master/build/shared/manpage.adoc

DIR_SRC := ./theft-protection

.PHONY: build
build:
	arduino --verify --board arduino:avr:nano:cpu=atmega328old --verbose theft-protection/theft-protection.ino

.PHONY: install
install:
	arduino --upload --board arduino:avr:nano:cpu=atmega328old --port /dev/ttyUSB0 --verbose theft-protection/theft-protection.ino

.PHONY: monitor
monitor:
	minicom -b 9600 -D /dev/ttyUSB0

.PHONY: format
format:
	find -type f \( -name '*.cpp' -o -name '*.hpp' \) -print0 | xargs -0 --max-args=1 --max-procs=8 clang-format -i

.PHONY: tags
tags:
	ctags -R \
		--sort=yes \
		--totals=yes \
		--languages=C++ \
		--extra=+f \
		${DIR_SRC}

.PHONY: clean
clean:
	rm -f ./tags
