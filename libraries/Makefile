
MCU=atmega168p
F_CPU=20000000

ARDUINO_DIR=..
ARDUINO_PORT='/dev/ttyACM*'
AVRDUDE_ARD_BAUDRATE=115200
AVRDUDE_ARD_PROGRAMMER=arduino

ARDUINO_LIBS:=LiquidCrystal
OUTPUTDIR=../$(MCU)_$(F_CPU)/libraries

SOURCES =  \
	EEPROM/EEPROM.cpp \
	EEPROM/EEPROM.h \
	EEPROM/examples/eeprom_clear/eeprom_clear.pde \
	EEPROM/examples/eeprom_read/eeprom_read.pde \
	EEPROM/examples/eeprom_write/eeprom_write.pde \
	EEPROM/keywords.txt \
	HardwareSerial/HardwareSerial.h \
	LiquidCrystal/examples/Autoscroll.pde \
	LiquidCrystal/examples/Blink.pde \
	LiquidCrystal/examples/Cursor.pde \
	LiquidCrystal/examples/Display.pde \
	LiquidCrystal/examples/HelloWorld.pde \
	LiquidCrystal/examples/Scroll.pde \
	LiquidCrystal/examples/SerialDisplay.pde \
	LiquidCrystal/examples/setCursor.pde \
	LiquidCrystal/examples/TextDirection.pde \
	LiquidCrystal/keywords.txt \
	LiquidCrystal/LiquidCrystal.h \
	Makefile \
	PWMMatrix/PWMMatrix.h \
	SerialBus/SerialBus.h \
	Tone/Tone.h

include $(ARDUINO_DIR)/Arduino.mk
