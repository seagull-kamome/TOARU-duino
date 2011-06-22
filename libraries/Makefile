
MCU=atmega168p
F_CPU=20000000

ARDUINO_DIR=..
ARDUINO_PORT='/dev/ttyACM*'
AVRDUDE_ARD_BAUDRATE=115200
AVRDUDE_ARD_PROGRAMMER=arduino

ARDUINO_LIBS:=LiquidCrystal
OUTPUTDIR=../$(MCU)_$(F_CPU)/libraries

SOURCES =  \
	HardwareSerial/HardwareSerial.cpp \
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
	LiquidCrystal/LiquidCrystal.cpp \
	LiquidCrystal/LiquidCrystal.h \
	Makefile \
	PWMMatrix/PWMMatrix.h \
	SerialBus/SerialBus.cpp \
	SerialBus/SerialBus.h \
	Tone/Tone.cpp \
	Tone/Tone.h

include $(ARDUINO_DIR)/Arduino.mk
