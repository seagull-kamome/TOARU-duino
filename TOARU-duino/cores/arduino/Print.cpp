/*
  Print.cpp - Print library for TOARU-duino
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
  
  This libray based on Arduino core library.
		Print.cpp - Base class that provides print() and println()
		Copyright (c) 2008 David A. Mellis.  All right reserved.
		Modified 23 November 2006 by David A. Mellis
 */
#include <cores/arduino/Print.h>

void Print::print(char const* str)
{
	while (*str) write(*str++);
}

void Print::print(uint8_t x, uint8_t fmt) { print((uint32_t)x, fmt); }
void Print::print(int8_t x, uint8_t fmt) { print((int32_t)x, fmt); }
void Print::print(uint16_t x, uint8_t fmt) { print((uint32_t)x, fmt); }
void Print::print(int16_t x, uint8_t fmt) { print((int32_t)x, fmt); };
void Print::print(uint32_t const& xx, uint8_t fmt)
{
	uint32_t x = xx;
	if (fmt == 0)
		write(fmt);
	else {

		char buf[8 * sizeof(long)];
		uint8_t n = 0;
		do {
			buf[n++] = x % fmt;
			x /= fmt;
		} while (x > 0);
		do {
			n--;
			write(buf[n] + (buf[n] < 10? '0' : ('A' - 10)) );
		} while (n);
	}
		
}

void Print::print(int32_t const& xx, uint8_t fmt)
{
	int32_t x = xx;
	if (fmt == DEC && x < 0) { write('-'); x = -x; }
	print((uint32_t)x, fmt);
}


void Print::print(double const& xx, uint8_t prec)
{
	double x = xx;
	if (x < 0.0) { write('-'); x = -x; }

	double r = 0.5;
	for (int i = 0; i < prec; ++i) r /= 10.0;
	x += r;

	print((uint32_t)x, DEC);
	if (prec > 0) {
		write('.');
		x -= (uint32_t)x;
		while (prec-- > 0) {
			x *= 10.0;
			int y = int(x);
			print(y);
			x -= y;
		}
	}
}

void print(float const& x, uint8_t prec) { print((double)x, prec); }


void Print::println(char const* str) { print(str); println(); }
void Print::println(uint8_t x, uint8_t fmt) { print(x, fmt); println(); }
void Print::println(int8_t x, uint8_t fmt) { print(x, fmt); println(); }
void Print::println(uint16_t x, uint8_t fmt) { print(x, fmt); println(); }
void Print::println(int16_t x, uint8_t fmt) { print(x, fmt); println(); }
void Print::println(uint32_t const&x, uint8_t fmt) { print(x, fmt); println(); }
void Print::println(int32_t const&x, uint8_t fmt) { print(x, fmt); println(); }
void Print::println(double const& x, uint8_t prec) { print(x, prec); println(); }
void Print::println(float const& x, uint8_t prec) { print(x, prec); println(); }

void Print::println()
{
	write('\r');
	write('\n');
}

