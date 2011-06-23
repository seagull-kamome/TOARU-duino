
MCU=atmega168p
F_CPU=20000000

ARDUINO_DIR=../..
ARDUINO_PORT='/dev/ttyACM*'
AVRDUDE_ARD_BAUDRATE=115200
AVRDUDE_ARD_PROGRAMMER=arduino

ARDUINO_LIBS:=LiquidCrystal
OUTPUTDIR=../$(MCU)_$(F_CPU)/libraries/LiquidCrystal

SOURCES =  \
	EEPROM.cpp \
	EEPROM.h \
	examples/eeprom_clear.pde \
	examples/eeprom_read.pde \
	examples/eeprom_write.pde \
	keywords.txt \
	Makefile

include $(ARDUINO_DIR)/Arduino.mk
