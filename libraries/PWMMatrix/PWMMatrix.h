/*
  PWMMatrix.h - PWM Based analog matrix library.
  Copyright (c) 2011 HATTORI, Hiroki.    All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA  
  
  
  Usage:
  	typedef PWMMatrix<cols, colpol, rows, rowpol, col_pins ..., row_pins ....> matrix;
  		cols, rows  : rows and cols of matrix.
  		colpol, rowpol : Polarity for idle.
  	
  	void setup()
  	{
  		matrix::begin();
  	}
  	
  	void loop()
  	{
  		matrix::setBrightness(row, col, brightness);
  		
  		matrix::update();
  		delay(100);
  	}
  	
 */
#include <WProgram.h>
#include <string.h>

template <int cols, int colpol, int rows, int rowpol, int pin0=-1, int pin1=-1, int pin2=-1, int pin3=-1, int pin4=-1, int pin5=01, int pin6=-1, int pin7=-1>
class PWMMatrix
{
public:
	PWMMatrix() {}

	enum { COLS = cols, ROWS = rows, NUM_PINS = cols + rows };

	void __inline__ begin()
	{
		current_col_ = cols - 1;
		off();
	}

	void update()
	{
		updateCol(current_col_, colpol);

		current_col_ = (current_col_ + 1) % cols;

		for (uint8_t r = 0; r < rows; r++)
			updateRow(r, brightness_[current_col_][r]);

		updateCol(current_col_, colpol? LOW:HIGH);
	}

	void off()
	{
		if (pin0 >= 0) pinMode(pin0, OUTPUT);
		if (pin1 >= 0) pinMode(pin1, OUTPUT);
		if (pin2 >= 0) pinMode(pin2, OUTPUT);
		if (pin3 >= 0) pinMode(pin3, OUTPUT);
		if (pin4 >= 0) pinMode(pin4, OUTPUT);
		if (pin5 >= 0) pinMode(pin5, OUTPUT);
		if (pin6 >= 0) pinMode(pin6, OUTPUT);
		if (pin7 >= 0) pinMode(pin7, OUTPUT);
		
		for (uint8_t r = 0; r < rows; r++) updateRow(r, rowpol);
		for (uint8_t c = 0; c < cols; c++) updateCol(c, colpol);
	}


	void __inline__ clear()
	{
		::memset(&brightness_, 0, rows * cols);
	}

	void __inline__ setBrightness(uint8_t row, uint8_t col, uint8_t brightness)
	{
		brightness_[col][row] = brightness;
	}

	void __inline__ getBrightness(uint8_t row, uint8_t col)
	{
		return brightness_[col][row];
	}

protected:
	static void __inline__ updateRow(uint8_t row, uint8_t val)
	{
		uint8_t const duty = (rowpol == LOW)? val : (255 - val);

		switch (row + cols)
		{
		case 0: if (pin0 >= 0) analogWrite(pin0, duty); break;
		case 1: if (pin1 >= 0) analogWrite(pin1, duty); break;
		case 2: if (pin2 >= 0) analogWrite(pin2, duty); break;
		case 3: if (pin3 >= 0) analogWrite(pin3, duty); break;
		case 4: if (pin4 >= 0) analogWrite(pin4, duty); break;
		case 5: if (pin5 >= 0) analogWrite(pin5, duty); break;
		case 6: if (pin6 >= 0) analogWrite(pin6, duty); break;
		case 7: if (pin7 >= 0) analogWrite(pin7, duty); break;
		}
	}

	static void __inline__ updateCol(uint8_t col, uint8_t val)
	{
		switch (col)
		{
		case 0: if (pin0 >= 0) digitalWrite(pin0, val); break;
		case 1: if (pin1 >= 0) digitalWrite(pin1, val); break;
		case 2: if (pin2 >= 0) digitalWrite(pin2, val); break;
		case 3: if (pin3 >= 0) digitalWrite(pin3, val); break;
		case 4: if (pin4 >= 0) digitalWrite(pin4, val); break;
		case 5: if (pin5 >= 0) digitalWrite(pin5, val); break;
		case 6: if (pin6 >= 0) digitalWrite(pin6, val); break;
		case 7: if (pin7 >= 0) digitalWrite(pin7, val); break;
		}
	}


protected:
	uint8_t current_col_;
	uint8_t brightness_[cols][rows];
};
