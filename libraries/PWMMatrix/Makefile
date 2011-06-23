
MCU=atmega168p
F_CPU=20000000

ARDUINO_DIR=../..
ARDUINO_PORT='/dev/ttyACM*'
AVRDUDE_ARD_BAUDRATE=115200
AVRDUDE_ARD_PROGRAMMER=arduino

ARDUINO_LIBS:=PWMMatrix
OUTPUTDIR=../../$(MCU)_$(F_CPU)/libraries/PWMMatrix

SOURCES =  \
	examples/Autoscroll.pde \
	examples/Blink.pde \
	examples/Cursor.pde \
	examples/Display.pde \
	examples/HelloWorld.pde \
	examples/Scroll.pde \
	examples/SerialDisplay.pde \
	examples/setCursor.pde \
	examples/TextDirection.pde \
	keywords.txt \
	LiquidCrystal.cpp \
	LiquidCrystal.h \
	Makefile

include $(ARDUINO_DIR)/Arduino.mk
