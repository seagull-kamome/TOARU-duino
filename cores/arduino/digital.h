/*
  digital.h - Digital I/O library for TOARU-duino
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
#if !defined(ARDUINO__CORE__GPIO_H)
#  define ARDUINO__CORE__GPIO_H

#  include <avr/io.h>
#  include <stdint.h>

enum { INPUT = 0, OUTPUT = 1,
		HIGH = 255, LOW = 0 };

struct High { enum { value = HIGH, inverse = LOW }; };
struct Low { enum { value = LOW, inverse = HIGH }; };
struct Input { enum { value = 0, inverse = 1 }; };
struct Output { enum { value = 1, inverse = 0 }; };

#  define pinno2ddrreg(x)		(*((x) < 8 ? &DDRD : (x) < 14 ? &DDRB : &DDRC))
#  define pinno2portreg(x)		(*((x) < 8 ? &PORTD : (x) < 14 ? &PORTB : &PORTC))
#  define pinno2pinreg(x)		(*((x) < 8 ? &PIND : (x) < 14 ? &PINB : &PINC))
#  define pinno2bit(x)			((x) < 8 ? (x) : (x) < 14 ? (x) - 8 : (x) - 14)



inline void pinMode_const(uint8_t pin, uint8_t mode) __attribute__ ((always_inline));
inline void pinMode_const(uint8_t pin, uint8_t mode)
{
	switch (mode)
	{
	case OUTPUT:
		pinno2ddrreg(pin) |= 1 << pinno2bit(pin);
		break;
	case INPUT:
		pinno2ddrreg(pin) &= ~(1 << pinno2bit(pin));
		break;
	}
}
void pinMode_noconst(uint8_t pin, uint8_t mode);
inline void pinMode(uint8_t pin, uint8_t mode) __attribute__ ((always_inline));
inline void pinMode(uint8_t pin, uint8_t mode)
{
	if (__builtin_constant_p(pin))
		pinMode_const(pin, mode);
	else
		pinMode_noconst(pin, mode);
}

void turnOffPWM_const(uint8_t pin) __attribute__ ((always_inline));
void turnOffPWM_const(uint8_t pin)
{
	switch (pin) {
	case 3:		/* OC2B	*/
		TCCR2A &= ~(1 << COM2B1);
		break;
	case 5:		/* OC0B */
		TCCR0A &= ~(1 << COM0B1);
		break;
	case 6:		/* OC0A */
		TCCR0A &= ~(1 << COM0A1);
		break;
	case 9:		/* OC1A */
		TCCR1A &= ~(1 << COM1A1);
		break;
	case 10:	/* OC1B */
		TCCR1A &= ~(1 << COM1B1);
		break;
	case 11:	/* OC2A */
		TCCR2A &= ~(1 << COM2A1);
		break;
	default:
		break;
	}
}

void turnOffPWM_noconst(uint8_t pin);


inline void digitalWrite_const(uint8_t pin, int val) __attribute__ ((always_inline));
inline void digitalWrite_const(uint8_t pin, int val)
{
	turnOffPWM_const(pin);
	if (val)
		pinno2portreg(pin) |= 1 << pinno2bit(pin);
	else
		pinno2portreg(pin) &= ~(1 << pinno2bit(pin));
}


void digitalWrite_noconst(uint8_t pin, uint8_t val);

inline void digitalWrite(uint8_t pin, int val) __attribute__ ((always_inline));
inline void digitalWrite(uint8_t pin, int val)
{
	if (__builtin_constant_p(pin)) digitalWrite_const(pin, val);
	else digitalWrite_noconst(pin, val);
}


void digitalToggle_noconst(uint8_t pin);
inline void digitalToggle(uint8_t pin) __attribute__ ((always_inline));
inline void digitalToggle(uint8_t pin)
{
	if (__builtin_constant_p(pin)) {
		trunOffPWM_const(pin);
		pinno2pinreg(pin) |= 1 << pinno2bit(pin);
	} else
		digitalToggle_noconst(pin);
}


bool digitalRead_noconst(uint8_t pin);
inline bool digitalRead(uint8_t pin) __attribute__ ((always_inline));
inline bool digitalRead(uint8_t pin)
{
	if (__builtin_constant_p(pin)) {
		trunOffPWM_const(pin);
		return ((pinno2pinreg(pin) & (1 << pinno2bit(pin)))?HIGH:LOW);
	} else 
		return digitalRead_noconst(pin);
}




unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);



/**
 IOポートアドレスとビットマスクの対を定義します。
*/
typedef uint16_t IOMask_t;

inline void ioMaskWrite(IOMask_t bit, uint8_t val)
{
	if (val == LOW)
		*(uint8_t volatile*)(bit >> 8) &= !(bit & 0x0f);
	else
		*(uint8_t volatile*)(bit >> 8) |= bit & 0x0f;
}

IOMask_t inline pinno2ddrmask(uint8_t pin)	{ return (uint8_t)pinno2ddrreg(pin) << 8 | (1 << pinno2bit(pin)); }
IOMask_t inline pinno2portmask(uint8_t pin)	{ return (uint8_t)pinno2portreg(pin) << 8 | (1 << pinno2bit(pin)); }
IOMask_t inline pinno2pinmask(uint8_t pin)	{ return (uint8_t)pinno2pinreg(pin) << 8 | (1 << pinno2bit(pin)); }

#endif

