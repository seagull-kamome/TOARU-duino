
MCU=atmega168p
F_CPU=20000000

ARDUINO_DIR=../..
ARDUINO_PORT='/dev/ttyACM*'
AVRDUDE_ARD_BAUDRATE=115200
AVRDUDE_ARD_PROGRAMMER=arduino

ARDUINO_LIBS:=
OUTPUTDIR=../../$(MCU)_$(F_CPU)/libraries/Dots

SOURCES =  \
	Dots.cpp \
	Dots.h \
	examples/flip_animation.pde \
	examples/heartbeat.pde \
	examples/hello_matrix.pde \
	examples/inverder.pde \
	examples/scroll_animation.pde \
	examples/wave_animation.pde \
	keywords.txt \
	Makefile

include $(ARDUINO_DIR)/Arduino.mk
