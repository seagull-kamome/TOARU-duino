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
 */
#if !defined(PWMMatrix_H)
#  define PWMMatrix_H

#  include <WProgram.h>
#  include <PinMapper.h>
#  include <string.h>
#  include <avr/pgmspace.h>

namespace PWMMatrix_impl
{
	/*
	 PWMMatrixの内部バッファの型と、その操作を定義する基底テンプレートクラスを定義します。
	*/
	template <uint8_t rows, uint8_t cols, bool analog> class PWMMatrixBuffer;
	
	/* 各点が0〜255の輝度を持つアナログのバッファ */
	template <uint8_t rows, uint8_t cols> class PWMMatrixBuffer<rows, cols, true>
	{
	private:
		uint8_t brightness_[rows][cols];
	public:
		void inline clear()
		{
			::memset(brightness_, 0, sizeof(brightness_));
		}
	
		void inline setBrightness(uint8_t const row, uint8_t const col, uint8_t const brightness)
		{
			brightness_[rows][cols] = brightness;
		}
		
		uint8_t inline getBrightness(uint8_t const row, uint8_t const col)
		{
			return brightness_[row][col];
		}
	};


	PROGMEM prog_char hexdigits[] __attribute__ ((weak)) = {
	        0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 
	        0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71 };


	/* 各点が HIGH/LOW の2値しか持たないデジタルのバッファ */
	template <uint8_t rows> class PWMMatrixBuffer_byte
	{
	private:
		uint8_t brightness_[rows];		// デジタルの場合、行数は最大8
	public:
		void inline clear()
		{
			::memset(brightness_, 0, sizeof(brightness_));
		}
	
		void inline setBrightness(uint8_t const row, uint8_t const col, uint8_t const brightness)
		{
			if (brightness > 127)
				brightness_[row] |= 1 << col;
			else
				brightness_[row] &= ~(1 << col);
		}
	
		uint8_t inline getBrightness(uint8_t const row, uint8_t const col)
		{
			return (brightness_[row] & (1 << col))? HIGH : LOW;
		}
		
		void inline write(uint8_t const row, uint8_t const value)
		{
			brightness_[row] = value;
		}
		
		void inline write(uint8_t const row, uint8_t const values[], uint8_t len)
		{
			if (len > rows - row) len = rows - row;
			::memcpy(brightness_ + row, values, len);
		}

		void inline write_P(uint8_t const row, uint8_t PROGMEM const* values, uint8_t len)
		{
			if (len > rows - row) len = rows - row;
			::memcpy_P(brightness_ + row, values, len);
		}
		
		void inline writeHexDigit(uint8_t row, uint8_t digit)
		{
			brightness_[row] = pgm_read_byte(&hexdigits[digit & 0x0f]);
		}
	};

	template <uint8_t rows, uint8_t cols>
	class PWMMatrixBuffer<rows, cols, false> : public PWMMatrixBuffer_byte<rows> {};
}


template <bool analog, uint8_t cols, uint8_t colpol, uint8_t rows, uint8_t rowpol, typename PinMapper_t>
class PWMMatrix : public PWMMatrix_impl::PWMMatrixBuffer<rows, cols, analog>
{
public:
	typedef PWMMatrix_impl::PWMMatrixBuffer<rows, cols, analog> super_t;

	PWMMatrix() {}

	enum { COLS = cols, ROWS = rows, NUM_PINS = cols + rows, UPDATE_CYCLE = 2 };

	void __inline__ begin()
	{
		current_row_ = 255;
		pinmapper_.pinModeAll(OUTPUT);
		off();
	}


	void update()
	{
		unsigned long t0 = millis();
		if (current_row_ == 255 || t0 > next_update_time_) {
			updateRow(current_row_, rowpol);

			current_row_ = (current_row_ + 1) % rows;
	
			for (uint8_t c = 0; c < cols; c++)
				updateCol(c, super_t::getBrightness(current_row_, c));
	
			updateRow(current_row_, rowpol? LOW:HIGH);
			
			next_update_time_ = t0 + UPDATE_CYCLE;
		}
	}


	void updateWithDelay(unsigned long ms)
	{
		unsigned long t0 = millis();
		do {
			update();
		} while (current_row_ != rows -1 || (millis() - t0) < ms);
	}

	/** すべてのピンをアイドル状態にします */
	void off()
	{
		for (uint8_t r = 0; r < rows; r++) updateRow(r, rowpol);
		for (uint8_t c = 0; c < cols; c++) updateCol(c, colpol);
	}


public:
	void inline updateCol(uint8_t col, uint8_t val)
	{
		if (analog)
			pinmapper_.analogWrite(col, (colpol == LOW)? val : (255 - val));
		else if (colpol == LOW)
			pinmapper_.digitalWrite(col, val);
		else
			pinmapper_.digitalWrite(col, val?LOW:HIGH);
	}

	void inline updateRow(uint8_t row, uint8_t val)
	{
		if (rowpol == LOW)
			pinmapper_.digitalWrite(row + cols, val);
		else
			pinmapper_.digitalWrite(row + cols, val?LOW:HIGH);
	}


protected:
	PinMapper_t pinmapper_;
	unsigned long next_update_time_;
	uint8_t current_row_;
};

#endif