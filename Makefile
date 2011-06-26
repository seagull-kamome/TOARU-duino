
SOURCES =  \
	examples/1.Basic/AnalogReadSerial.pde \
	examples/1.Basic/BareMinimum.pde \
	examples/1.Basic/Blink.pde \
	examples/1.Basic/DigitalReadSerial.pde \
	examples/1.Basic/Fade.pde \
	examples/2.Digital/BlinkWithoutDelay.pde \
	examples/2.Digital/Button.pde \
	examples/2.Digital/Debounce.pde \
	examples/2.Digital/StateChangeDetection.pde \
	examples/3.Analog/AnalogInput.pde \
	examples/3.Analog/AnalogWriteMega.pde \
	examples/3.Analog/Calibration.pde \
	examples/3.Analog/Fading.pde \
	examples/3.Analog/Smoothing.pde \
	libraries/EEPROM/examples/eeprom_clear.pde \
	libraries/EEPROM/examples/eeprom_read.pde \
	libraries/EEPROM/examples/eeprom_write.pde \
	libraries/LiquidCrystal/examples/Autoscroll.pde \
	libraries/LiquidCrystal/examples/Blink.pde \
	libraries/LiquidCrystal/examples/Cursor.pde \
	libraries/LiquidCrystal/examples/Display.pde \
	libraries/LiquidCrystal/examples/HelloWorld.pde \
	libraries/LiquidCrystal/examples/Scroll.pde \
	libraries/LiquidCrystal/examples/SerialDisplay.pde \
	libraries/LiquidCrystal/examples/setCursor.pde \
	libraries/LiquidCrystal/examples/TextDirection.pde \
	libraries/PWMMatrix/examples/fade.pde

all:
	for f in $(filter %.pde,$(SOURCES)); do \
		./arduino-upload -n -b pro-20mhz-p -o dist.pro-20mhz-p $$f; \
	done
