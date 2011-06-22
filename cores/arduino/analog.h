/*
  analog.h - Analog I/O library for TOARU-duino
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
#if !defined(ARDUINO__CORE__ADC_H)
#  define ARDUINO__CORE__ADC_H
 
#  include <avr/io.h>
#  include <cores/arduino/digital.h>

enum { INTERNAL = 3, DEFAULT = 1, EXTERNAL = 0 };
 
static inline void analogReference(uint8_t mode)
{
 	ADMUX = ADMUX & ~(0x03 << REFS0) | (mode & 0x03) << REFS0;
}
 
 
static inline void analogSetChannel(uint8_t pin)
{
#if defined(__AVR_ATmega168p)
	ADMUX = ADMUX & 0x0f | (pin & 0x0f);
#else
	ADMUX = ADMUX & 0x07 | (pin & 0x07);
#endif
}
static inline void analogStartConversion() { ADCSRA |= 1 << ADSC; }
static inline void analogWaitForConversion() { while (ADCSRA & (1 << ADSC)); }
static inline uint16_t analogResult() { uint8_t low = ADCL; return (ADCH << 8) | low; }

uint16_t analogRead(uint8_t pin);
void analogWrite(uint8_t pin, int val);


static __inline__ void analogWrite_const(uint8_t pin, uint8_t val) 
{
	pinMode_const(pin, OUTPUT);
	if (val == 0)
		digitalWrite_const(pin, LOW);
	else if (val == 255)
		digitalWrite_const(pin, HIGH);
	else switch (pin)
	{
	case 3:		// OC2B
		TCCR2A |= 1 << COM2B1;
		OCR2B = val;
		break;

	case 5:		// OC0B
		TCCR0A |= 1 << COM0B1;
		OCR0B = val;
		break;

	case 6:		// OC0A
		TCCR0A |= 1 << COM0A1;
		OCR0A = val;
		break;

	case 9:		// OC1A
		TCCR1A |= 1 << COM1A1;
		OCR1A = val;
		break;

	case 10:	// OC1B
		TCCR1A |= 1 << COM1B1;
		OCR1B = val;
		break;

	case 11:	// OC2A
		TCCR2A |= 1 << COM2A1;
		OCR2A = val;
		break;

	default:
		digitalWrite_const(pin, val<128? LOW : HIGH);
		break;
	}
}

void analogWrite_noconst(uint8_t pin, uint8_t val);

#  define analogWrite(x, y)				\
	do {								\
		if (__builtin_constant_p(x))	\
			analogWrite_const(x,y);		\
		else							\
			analogWrite_noconst(x,y);	\
	} while (0)

#endif
