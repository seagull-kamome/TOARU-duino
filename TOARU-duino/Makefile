
MCU=atmega168p
F_CPU=20000000

BASIC=Blink Fade BareMinimum DigitalReadSerial AnalogReadSerial
DIGITAL=BlinkWithoutDelay Button Debounce StateChangeDetection toneKeyboard toneMelody toneMultiple tonePitchFollower
ANALOG=AnalogInOutSerial AnalogInput Fading Smoothing

examples: examples-basic examples-digital examples-analog

examples-basic:
	for i in $(BASIC); do \
		make -C examples/1.Basic -f ../../Arduino.mk ARDUINO_DIR=`realpath .` MCU=$(MCU) F_CPU=$(F_CPU) TARGET=$$i depends all; \
	done
examples-digital:
	for i in $(DIGITAL); do \
		make -C examples/2.Digital -f ../../Arduino.mk ARDUINO_DIR=`realpath .` MCU=$(MCU) F_CPU=$(F_CPU) TARGET=$$i depends all; \
	done
examples-analog:
	for i in $(ANALOG); do \
		make -C examples/3.Analog -f ../../Arduino.mk ARDUINO_DIR=`realpath .` MCU=$(MCU) F_CPU=$(F_CPU) TARGET=$$i depends all; \
	done

clean:
	for i in $(BASIC); do \
		make -C examples/1.Basic -f ../../Arduino.mk ARDUINO_DIR=`realpath .` MCU=$(MCU) F_CPU=$(F_CPU) TARGET=$$i clean; \
	done
	for i in $(DIGITAL); do \
		make -C examples/2.Digital -f ../../Arduino.mk ARDUINO_DIR=`realpath .` MCU=$(MCU) F_CPU=$(F_CPU) TARGET=$$i clean; \
	done
	for i in $(ANALOG); do \
		make -C examples/3.Analog -f ../../Arduino.mk ARDUINO_DIR=`realpath .` MCU=$(MCU) F_CPU=$(F_CPU) TARGET=$$i clean; \
	done
	rm -rf 1.Basic/build-cli
	rm -rf 2.Digital/build-cli

orig-examples:
	for i in $(BASIC); do \
		mkdir -p examples/1.Basic/$$i; \
		cp examples/1.Basic/$$i.pde examples/1.Basic/$$i; \
		make -C examples/1.Basic/$$i -f /usr/share/arduino/Arduino.mk ARDUINO_DIR=/usr/share/arduino MCU=$(MCU) F_CPU=$(F_CPU) TARGET=$$i depends all; \
	done


size-compare: examples orig-examples
	@for i in $(BASIC); do \
		echo "";			\
		echo $$i;		\
		echo "--------------------------------------------------";	\
		avr-size examples/1.Basic/$$i.build_tmp/$$i.elf;		\
		avr-size examples/1.Basic/$$i/build-cli/$$i.elf;			\
	done;

raw_upload:
	make -C examples/$(TGTDIR) -f ../../Arduino.mk ARDUINO_DIR=`realpath .` ARDUINO_PORT='/dev/ttyACM*' AVRDUDE_ARD_BAUDRATE=115200 AVRDUDE_ARD_PROGRAMMER=arduino MCU=$(MCU) F_CPU=$(F_CPU) TARGET=$(TARGET) raw_upload

.PHONY: examples
