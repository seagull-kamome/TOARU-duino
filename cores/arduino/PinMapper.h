/*
  PinMapper.h - Pin map library for TOARU-duino
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
#if !defined(PINMAPPER_H)
#  define PINMAPPER_H

#  include <digital.h>
#  include <analog.h>

#  define OP1(f,a)		f(a,0) f(a,1) f(a,2) f(a,3) f(a,4) f(a,5) f(a,6) f(a,7) f(a,8) f(a,9)
#  define OP(f)			OP1(f,0) OP1(f,1)


template <
	int pin00,    int pin01=-1, int pin02=-1, int pin03=-1, int pin04=-1, int pin05=-1, int pin06=-1, int pin07=-1,
	int pin08=-1, int pin09=-1, int pin10=-1, int pin11=-1, int pin12=-1, int pin13=-1, int pin14=-1, int pin15=-1,
	int pin16=-1, int pin17=-1, int pin18=-1, int pin19=-1>
struct PinMapper
{
	enum {
#  define f(a,b)		+ ((pin##a##b >= 0) ? 1 : 0)
		NUM_PINS = 0 OP(f),
#  undef f
	};
	
	uint32_t inline pinmask() const
	{
#  define f(a,b)		| ((pin##a##b >= 0) ? 1 << (pin##a##b) : 0)
		return 0 OP(f);
#  undef f
	};

	void inline pinModeAll(int mode)
	{
		uint32_t msk = pinmask();

		uint8_t pins = msk & 0xff;
		if (pins != 0) {
			if (mode == OUTPUT)
				pinno2ddrreg(0) |= pins;
			else
				pinno2ddrreg(0) &= ~pins;
		}

		pins = (msk >> 8) & 0x3f;
		if (pins != 0) {
			if (mode == OUTPUT)
				pinno2ddrreg(8) |= pins;
			else
				pinno2ddrreg(8) &= ~pins;
		}
		
		pins = (msk >> 14) & 0x3f;
		if (pins != 0) {
			if (mode == OUTPUT)
				pinno2ddrreg(14) |= pins;
			else
				pinno2ddrreg(14) &= ~pins;
		}
	}

	void inline pinMode(int x, int mode)
	{
#  define f(a,b)		case ((a) * 10 + (b)):	if (pin##a##b >= 0) pinMode(pin##a##b, mode); break;
		switch (x) { OP(f) }
#  undef f
	}


	void inline digitalWriteAll(int value)
	{
		uint32_t msk = pinmask();

		uint8_t pins = msk & 0xff;
		if (pins != 0) {
			if (value != LOW)
				pinno2portreg(0) |= pins;
			else
				pinno2portreg(0) &= ~pins;
		}

		pins = (msk >> 8) & 0x3f;
		if (pins != 0) {
			if (value != LOW)
				pinno2portreg(8) |= pins;
			else
				pinno2portreg(8) &= ~pins;
		}
		
		pins = (msk >> 14) & 0x3f;
		if (pins != 0) {
			if (value != LOW)
				pinno2portreg(14) |= pins;
			else
				pinno2portreg(14) &= ~pins;
		}
	}

	void digitalWrite(int const x, int const value)
	{
#  define f(a,b)		if ((a) * 10 + (b) == x && pin##a##b >= 0) digitalWrite_const(pin##a##b, value); else
		OP(f) ;
#  undef f
	}
	
	bool digitalRead(int const x)
	{
#  define f(a,b)		case ((a) * 10 + (b)):	if (pin##a##b >= 0) return digitalRead(pin##a##b);
		switch (x) { OP(f) }
#  undef f
	}

	void write(uint8_t const offs, uint8_t x, uint8_t const len)
	{
		for (int i = offs; i < len + offs; i++) {
			digitalWrite(i, (x & 1));
			x >>= 1;
		}
	}

	void write(uint8_t const offs, uint16_t x, uint8_t const len)
	{
		for (int i = offs; i < len + offs; i++) {
			digitalWrite(i, (x & 1));
			x >>= 1;
		}
	}

	void write(uint8_t const offs, uint32_t x, uint8_t const len)
	{
		for (int i = offs; i < len + offs; i++) {
			digitalWrite(i, (x & 1));
			x >>= 1;
		}
	}

	void analogWrite(int pin, uint8_t value)
	{
#  define f(a, b)	case ((a) * 10 + b): if (pin##a##b >= 0) analogWrite(pin##a##b, value); break;
		switch (pin) { OP(f) }
#  undef f
	}

	uint16_t analogRead(int pin)
	{
#  define f(a, b)	case ((a) * 10 + b): return (pin##a##b >= 0)? analogRead(pin##a##b) : 0;
		switch (pin) { OP(f) }
#  undef f
	}
};


#  undef OP1
#  undef OP

#endif
