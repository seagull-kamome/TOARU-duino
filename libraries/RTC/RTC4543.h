/*
  RTC/RTC4543.h - RTC library for EPSON RTC-4543SA/SB serial RTC.
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

#ifndef RTC_RTC4543_H
#  define RTC_RTC4543_H


#include <libraries/SerialBus/SerialBus.h>


struct DateTime_t
{
	uint8_t		sec;
	uint8_t		min;
	uint8_t		hour;
	uint8_t		wday;
	uint8_t		day;
	uint8_t		mon;
	uint8_t		year;

	void fmt(char* p, uint8_t x) { p[0] = (x >> 4 & 0x0f) + '0'; p[1] = (x & 0x0f) + '0'; }
	void format(char buf[14])
	{
		fmt(buf + 0, year);
		fmt(buf + 2, mon);
		fmt(buf + 4, day);
		buf[6] = wday + '0';
		fmt(buf + 7, hour);
		fmt(buf + 9, min);
		fmt(buf + 11, sec);
		buf[14] = '\0';
	}
	
	void parse(char const* p)
	{
		year = (p[0] - '0') << 4 + (p[1] - '0');
		mon = (p[2] - '0') << 4 + (p[3] - '0');
		day = (p[4] - '0') << 4 + (p[5] - '0');
		wday = p[6] - '0';
		hour = (p[7] - '0') << 4 + (p[8] - '0');
		min= (p[9] - '0') << 4 + (p[10] - '0');
		sec = (p[11] - '0') << 4 + (p[12] - '0');
	}
};


template <int pinCE, int pinWR, typename SBUS>
class RTC4543 : protected SerialDevice<SBUS, LOW, typename SBUS::OutputFirst, typename SBUS::LSBFirst>
{
protected:

	static PinMapper<pinCE, pinWR> ctrl_pins_;
	typedef SerialDevice<SBUS, LOW, typename SBUS::OutputFirst, typename SBUS::LSBFirst> super;

public:
	inline RTC4543() {}
	inline ~RTC4543() {}


	static void begin()
	{
		super::begin();
		
		pinMode(pinCE, OUTPUT);
		pinMode(pinWR, OUTPUT);
		digitalWrite(pinCE, LOW);
		digitalWrite(pinWR,LOW);
	}



	static void read(DateTime_t* tm)
	{
		digitalWrite(pinCE, HIGH);

		tm->sec = super::shift_in(8, 1) & 0x7f;
		tm->min = super::shift_in(8, 1) & 0x7f;
		tm->hour = super::shift_in(8, 1) & 0x3f;
		tm->wday = super::shift_in(4, 1) >> 4 & 0x07;
		tm->day = super::shift_in(8, 1) & 0x3f;
		tm->mon = super::shift_in(8, 1) & 0x1f;
		tm->year = super::shift_in(8, 1);

		digitalWrite(pinCE, LOW);
	}


	static void write(DateTime_t const& tm) { write(tm.year, tm.mon, tm.day, tm.wday, tm.hour, tm.min, tm.sec);	}

	static void write(uint8_t year, uint8_t month, uint8_t day, uint8_t wday, uint8_t hour, uint8_t min, uint8_t sec)
	{
		digitalWrite(pinWR, HIGH);
		digitalWrite(pinCE, HIGH);
		
		super::shift_out(sec, 8, 1);
		super::shift_out(min, 8, 1);
		super::shift_out(hour, 8, 1);
		super::shift_out(wday, 4, 1);
		super::shift_out(day, 8, 1);
		super::shift_out(month, 8, 1);
		super::shift_out(year, 8, 1);
		
		digitalWrite(pinCE, LOW);
		digitalWrite(pinWR, LOW);
	}
};


#endif
