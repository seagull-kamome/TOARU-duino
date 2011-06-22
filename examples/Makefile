
MCU=atmega168p
F_CPU=20000000

ARDUINO_DIR=..
ARDUINO_PORT='/dev/ttyACM*'
AVRDUDE_ARD_BAUDRATE=115200
AVRDUDE_ARD_PROGRAMMER=arduino

OUTPUTDIR=../$(MCU)_$(F_CPU)/examples

SOURCES =  \
	1.Basic/AnalogReadSerial.pde \
	1.Basic/BareMinimum.pde \
	1.Basic/Blink.pde \
	1.Basic/DigitalReadSerial.pde \
	1.Basic/Fade.pde \
	2.Digital/BlinkWithoutDelay.pde \
	2.Digital/Button.pde \
	2.Digital/Debounce.pde \
	2.Digital/pitches.h \
	2.Digital/StateChangeDetection.pde \
	3.Analog/AnalogInOutSerial.pde \
	3.Analog/AnalogInput.pde \
	3.Analog/AnalogWriteMega.pde \
	3.Analog/Calibration.pde \
	3.Analog/Fading.pde \
	3.Analog/Smoothing.pde \
	Makefile

include $(ARDUINO_DIR)/Arduino.mk
