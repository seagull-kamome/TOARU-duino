#ifndef LiquidCrystal_h
#define LiquidCrystal_h

#include <WProgram.h>
#include <inttypes.h>
#include <PinMapper.h>
#include <avr/pgmspace.h>
#include "Print.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00


namespace LiquidCrystal_impl
{
	extern uint8_t PROGMEM row_offsets[4];
}


template <uint8_t cols, uint8_t rows, uint8_t charsize, int rs, int en, int d4, int d5, int d6, int d7>
class LiquidCrystal : public Print
{
protected:
	PinMapper<rs, en, d4, d5, d6, d7> pinmap_;

	uint8_t displaycontrol_;
	uint8_t displaymode_;
	uint8_t currline_;
	
	enum { DISPLAY_FUNCTION = LCD_4BITMODE | (rows > 1? LCD_2LINE:LCD_1LINE) | charsize };

public:
	enum { ROWS = rows, COLS = cols };

	LiquidCrystal() : currline_(0)
	{
	}

	// When the display powers up, it is configured as follows:
	//
	// 1. Display clear
	// 2. Function set: 
	//    DL = 1; 8-bit interface data 
	//    N = 0; 1-line display 
	//    F = 0; 5x8 dot character font 
	// 3. Display on/off control: 
	//    D = 0; Display off 
	//    C = 0; Cursor off 
	//    B = 0; Blinking off 
	// 4. Entry mode set: 
	//    I/D = 1; Increment by 1 
	//    S = 0; No shift 
	//
	// Note, however, that resetting the Arduino doesn't reset the LCD, so we
	// can't assume that its in that state when a sketch starts (and the
	// LiquidCrystal constructor is called).
	void begin()
	{
		pinmap_.pinModeAll(OUTPUT);

		// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
		// according to datasheet, we need at least 40ms after power rises above 2.7V
		// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
		delayMicroseconds(50000); 

		// Now we pull both RS and R/W low to begin commands
		pinmap_.digitalWriteAll(LOW);

		// this is according to the hitachi HD44780 datasheet
		// page 45 figure 23

		// Send function set command sequence
		command(LCD_FUNCTIONSET | DISPLAY_FUNCTION); delayMicroseconds(4500);
		command(LCD_FUNCTIONSET | DISPLAY_FUNCTION); delayMicroseconds(150);
		command(LCD_FUNCTIONSET | DISPLAY_FUNCTION);

		// finally, set # lines, font size, etc.
		command(LCD_FUNCTIONSET | DISPLAY_FUNCTION);  

		// turn the display on with no cursor or blinking default
		displaycontrol_ = LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
		command(displaycontrol_);

		clear();

		// Initialize to default text direction (for romance languages)
		displaymode_ = LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
		command(displaymode_); // set the entry mode
	}

	void clear()
	{
		command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
		delayMicroseconds(2000);  // this command takes a long time!
	}

	void home()
	{
		command(LCD_RETURNHOME);  // set cursor position to zero
		delayMicroseconds(2000);  // this command takes a long time!
	}

	void noDisplay()	{ command(displaycontrol_ &= ~LCD_DISPLAYON); }
	void display()		{ command(displaycontrol_ |= LCD_DISPLAYON); }
	void noBlink()		{ command(displaycontrol_ &= ~LCD_BLINKON); }
	void blink()		{ command(displaycontrol_ &= ~LCD_BLINKON); }
	void noCursor()		{ command(displaycontrol_ &= ~LCD_CURSORON); }
	void cursor()		{ command(displaycontrol_ |= LCD_CURSORON); }

	void scrollDisplayLeft()	{ command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT); }
	void scrollDisplayRight()	{ command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT); }
	void leftToRight()			{ command(displaymode_ |= LCD_ENTRYLEFT); }
	void rightToLeft()			{ command(displaymode_ &= ~LCD_ENTRYLEFT); }

	void autoscroll()			{ command(displaymode_ |= LCD_ENTRYSHIFTINCREMENT); }
	void noAutoscroll()			{ command(displaymode_ &= ~LCD_ENTRYSHIFTINCREMENT); }

	void createChar(uint8_t location, uint8_t charmap[])
	{
		location &= 0x7; // we only have 8 locations 0-7
		command(LCD_SETCGRAMADDR | (location << 3));
		for (int i=0; i<8; i++)
			write(charmap[i]);
	}
	void setCursor(uint8_t col, uint8_t row)
	{
		if (row >= rows)
			row = rows - 1;    // we count rows starting w/0
  
		command(LCD_SETDDRAMADDR | (col + pgm_read_byte(&LiquidCrystal_impl::row_offsets[row])));
	}

	virtual void write(uint8_t value)	{ digitalWrite(rs, HIGH); send(value); }
	void command(uint8_t value)			{ digitalWrite(rs, LOW); send(value); }

private:
	void send(uint8_t value)
	{
		write4bits(value >> 4);
		write4bits(value);
	}

	void write4bits(uint8_t x)
	{
		pinmap_.write(2, x, 4);

		digitalWrite(en, LOW);
		delayMicroseconds(1);    
		digitalWrite(en, HIGH);
		delayMicroseconds(1);    // enable pulse must be >450ns
		digitalWrite(en, LOW);
		delayMicroseconds(100);   // commands need > 37us to settle
	}
};

#endif
