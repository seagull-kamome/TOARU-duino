//
//
//
#ifndef RTC4543_H
#  define RTC4543_H


#include "SerialBUS.h"


struct RTC4543_Base
{
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
		void format(char buf[13])
		{
			fmt(buf + 0, year);
			fmt(buf + 2, mon);
			fmt(buf + 4, day);
			buf[6] = wday + '0';
			fmt(buf + 7, hour);
			fmt(buf + 9, min);
			fmt(buf + 11, sec);
		}
	};
};


template <int pinCE, int pinWR, typename SBUS>
class RTC4543 : public RTC4543_Base
{
protected:
	inline RTC4543() {}
	inline ~RTC4543() {}

public:
	typedef SerialDevice<SBUS, typename SBUS::Low, typename SBUS::OutputFirst, typename SBUS::LSBFirst> SDev;

	static void begin()
	{
		pinMode(pinCE, OUTPUT);
		pinMode(pinWR, OUTPUT);
		digitalWrite(pinCE, LOW);
		digitalWrite(pinWR, LOW);
	}



	static void read(DateTime_t* tm)
	{
		digitalWrite(pinCE, HIGH);

		tm->sec = SDev::shift_in(8) & 0x7f;
		tm->min = SDev::shift_in(8) & 0x7f;
		tm->hour = SDev::shift_in(8) & 0x3f;
		tm->wday = SDev::shift_in(4) >> 4 & 0x07;
		tm->day = SDev::shift_in(8) & 0x3f;
		tm->mon = SDev::shift_in(8) & 0x1f;
		tm->year = SDev::shift_in(8);

		digitalWrite(pinCE, LOW);
	}


	static void write(DateTime_t const& tm) { write(tm.year, tm.mon, tm.day, tm.wday, tm.hour, tm.min, tm.sec);	}

	static void write(uint8_t year, uint8_t month, uint8_t day, uint8_t wday, uint8_t hour, uint8_t min, uint8_t sec)
	{
		digitalWrite(pinWR, HIGH);
		digitalWrite(pinCE, HIGH);
		
		SDev::shift_out(sec, 8);
		SDev::shift_out(min, 8);
		SDev::shift_out(hour, 8);
		SDev::shift_out(wday, 4);
		SDev::shift_out(day, 8);
		SDev::shift_out(month, 8);
		SDev::shift_out(year, 8);
		
		digitalWrite(pinCE, LOW);
		digitalWrite(pinWR, LOW);
	}
};


#endif