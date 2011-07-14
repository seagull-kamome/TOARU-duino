/*
  Print.h - Print library for TOARU-duino
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
#if !defined(ARDUINO__CORE__PRINT_H)
#  define ARDUINO__CORE__PRINT_H

#  include <stdint.h>

enum { BYTE = 0, BIN = 2, OCT = 8, DEC= 10, HEX = 16 };

struct Print
{
	virtual void write(uint8_t ch) = 0;
	virtual void write(char const* str);
	virtual void write(uint8_t const* p, uint8_t n);

	void print(char const* str) { write(str); }
	
	void print(uint8_t x, uint8_t fmt=DEC);
	void print(int8_t x, uint8_t fmt=DEC);
	void print(uint16_t x, uint8_t fmt=DEC);
	void print(int16_t x, uint8_t fmt=DEC);
	void print(uint32_t const&x, uint8_t fmt=DEC);
	void print(int32_t const&x, uint8_t fmt=DEC);
	void print(double const& x, uint8_t prec);
	void print(float const& x, uint8_t prec);

	void println(char const* str);
	void println(uint8_t x, uint8_t fmt=DEC);
	void println(int8_t x, uint8_t fmt=DEC);
	void println(uint16_t x, uint8_t fmt=DEC);
	void println(int16_t x, uint8_t fmt=DEC);
	void println(uint32_t const&x, uint8_t fmt=DEC);
	void println(int32_t const&x, uint8_t fmt=DEC);
	void println(double const& x, uint8_t prec);
	void println(float const& x, uint8_t prec);
	
	void println();
};

#endif
