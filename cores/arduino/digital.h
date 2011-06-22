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
		HIGH = 1, LOW = 0 };

struct High { enum { value = HIGH, inverse = LOW }; };
struct Low { enum { value = LOW, inverse = HIGH }; };
struct Input { enum { value = 0, inverse = 1 }; };
struct Output { enum { value = 1, inverse = 0 }; };

#  define pinno2ddrreg(x)		(*((x) < 8 ? &DDRD : (x) < 14 ? &DDRB : &DDRC))
#  define pinno2portreg(x)		(*((x) < 8 ? &PORTD : (x) < 14 ? &PORTB : &PORTC))
#  define pinno2pinreg(x)		(*((x) < 8 ? &PIND : (x) < 14 ? &PINB : &PINC))
#  define pinno2bit(x)			((x) < 8 ? (x) : (x) < 14 ? (x) - 8 : (x) - 14)


#  define pinMode_const(pin, mode)	do {						\
		switch (mode)											\
		{														\
		case OUTPUT:											\
			pinno2ddrreg(pin) |= 1 << pinno2bit(pin);			\
			break;												\
		case INPUT:												\
			pinno2ddrreg(pin) &= ~(1 << pinno2bit(pin));		\
			break;												\
		}														\
	} while (0)


void pinMode_noconst(uint8_t pin, uint8_t mode);

#   define pinMode(pin, mode)										\
	do {															\
		if (__builtin_constant_p(pin))								\
			pinMode_const(pin, mode);								\
		else														\
			pinMode_noconst(pin, mode);								\
	 } while (0)


#  define turnOffPWM_const(pin) do {								\
		switch (pin)												\
		{															\
		case 3:		/* OC2B	*/										\
			TCCR2A &= ~(1 << COM2B1);								\
			break;													\
		case 5:		/* OC0B */										\
			TCCR0A &= ~(1 << COM0B1);								\
			break;													\
		case 6:		/* OC0A */										\
			TCCR0A &= ~(1 << COM0A1);								\
			break;													\
		case 9:		/* OC1A */										\
			TCCR1A &= ~(1 << COM1A1);								\
			break;													\
		case 10:	/* OC1B */										\
			TCCR1A &= ~(1 << COM1B1);								\
			break;													\
		case 11:	/* OC2A */										\
			TCCR2A &= ~(1 << COM2A1);								\
			break;													\
		default:													\
			break;													\
		}															\
	} while (0)

void turnOffPWM_noconst(uint8_t pin);

#  define digitalWrite_const(pin, val)	do { 				\
	turnOffPWM_const(pin);									\
	if (val)												\
		pinno2portreg(pin) |= 1 << pinno2bit(pin);			\
	else													\
		pinno2portreg(pin) &= ~(1 << pinno2bit(pin));		\
	} while (0)

void digitalWrite_noconst(uint8_t pin, uint8_t val);

#   define digitalWrite(pin, val)									\
	do {															\
		if (__builtin_constant_p(pin)) digitalWrite_const(pin, val);	\
		else digitalWrite_noconst(pin, val);						\
	 } while (0)



#  define digitalRead_const(pin) ((pinno2pinreg(pin) & (1 << pinno2bit(pin)))?HIGH:LOW)

bool digitalRead_noconst(uint8_t pin);

#   define digitalRead(pin)	(__builtin_constant_p(pin)? digitalRead_const(pin) : digitalRead_noconst(pin))



unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);

#endif

